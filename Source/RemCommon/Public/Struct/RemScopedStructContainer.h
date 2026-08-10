// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "RemNotNull.h"
#include "Enum/RemHelperEnum.h"

#include "HAL/UnrealMemory.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/AssertionMacros.h"
#include "StructUtils/StructView.h"
#include "Templates/AlignmentTemplates.h"
#include "Templates/TypeCompatibleBytes.h"
#include "Templates/UnrealTemplate.h"

namespace Rem::Struct
{
namespace Private
{
/**
 * Per-element metadata: the struct type and a pointer to the element data.
 * "Metadata" = the bookkeeping entry kept for each element (type + location);
 * entries are embedded in the chunk memory and are allocated/released together
 * with the chunk (see TScopedStructContainer::FChunk).
 * Both pointers are invariant non-null: elements are only ever added through
 * Add/Emplace, which always supply a valid type and a valid slot.
 */
struct FItemMeta
{
    TNotNull<const UScriptStruct*> ScriptStruct;
    TNotNull<uint8*> Memory;
};
}

/**
 * Call-scoped container of heterogeneous USTRUCT: append during one function call,
 * access by index or iteration, everything is released when the container goes out of
 * scope.
 *
 * - An embedded inline block (InlineBytes, on the stack) is used first: zero allocations
 *   within the budget. Overflow is allocated in chunks of OverflowChunkBytes.
 * - Elements never move: views issued before an overflow allocation stay valid.
 * - The metadata table (one FItemMeta per element) is embedded in the chunk memory;
 *   the container has no secondary containers.
 * - Two add paths: Emplace<T> (in-place construction, zero copy) and Add (reflection
 *   initialize + copy, covers runtime/BP structs).
 * - Type lookup: FindStructView / FindConstStructView overloads live in
 *   RemScopedStructContainer.inl (include it alongside this header).
 *
 * Example:
 *    Rem::Struct::TScopedStructContainer<> Pool;
 *    auto S = Pool.Emplace<FMyStruct>(); // TNotNull<FMyStruct*>
 *    S->SomeField = 1;
 *    Pool.Add(FConstStructView::Make(OtherStruct));
 *    for (FStructView View : Pool) { ... }
 *
 * @note Non-copyable, non-movable; single-threaded use only; v1 does not provide Reset().
 */
template <int32 InlineBytes = 1024, int32 OverflowChunkBytes = 4096>
class TScopedStructContainer
{
    static_assert(InlineBytes >= 64, "TScopedStructContainer: InlineBytes is too small.");
    static_assert(OverflowChunkBytes >= 64, "TScopedStructContainer: OverflowChunkBytes is too small.");

    // Design constants.
    // BlockAlignment: 16 is the alignment ceiling of reflected USTRUCT data
    // (FVector/FQuat/FMatrix are alignas(16), BP structs do not exceed it) and matches
    // the Malloc alignment used for heap chunks; it also covers FItemMeta (needs only 8).
    static constexpr auto BlockAlignment  = 16;
    // ChunkSlackBytes: extra bytes in a fresh chunk so the worst combination (e.g. an
    // alignas(32) element larger than OverflowChunkBytes) always fits; per-element slots
    // are still aligned individually via AlignDown in TryAllocate.
    static constexpr auto ChunkSlackBytes = 64;

    /**
     * A chunk is a single memory block with two bump regions that grow toward each other:
     *   - metadata region: grows up from the chunk head (one FItemMeta per element)
     *   - data region:     grows down from the chunk tail (the struct instances)
     * New elements are only placed in the tail chunk; append-only means all older chunks
     * are full and never touched again, so elements never move and all issued views stay
     * valid for the whole container lifetime.
     * Metadata/DataLowest/MemoryEnd are invariant non-null in a live chunk; Next/Prev stay
     * raw pointers because they terminate the chunk chain with nullptr.
     */
    struct FChunk
    {
        TNotNull<Private::FItemMeta*> Metadata; // metadata region start (bump up)
        int32 MetadataNum{};                    // number of elements in this chunk
        TNotNull<uint8*> DataLowest;            // lowest used byte of the data region (bump down)
        TNotNull<uint8*> MemoryEnd;             // one past the end of the block
        FChunk* Next{};                         // chain terminator
        FChunk* Prev{};                         // chain terminator

        FChunk(Private::FItemMeta* InMetadata, uint8* InDataLowest, uint8* InMemoryEnd)
            : Metadata(MakeNotNull(InMetadata))
          , DataLowest(MakeNotNull(InDataLowest))
          , MemoryEnd(MakeNotNull(InMemoryEnd))
        {
        }
    };

    // Embedded inline block: TAlignedBytes pins the base to BlockAlignment, but
    // per-element slots are still aligned individually via AlignDown in TryAllocate,
    // so this is not a hard limit for exotic alignments. Plus the chunk-chain head
    // and tail; the inline chunk is both head and tail until the first overflow.
    TAlignedBytes<InlineBytes, BlockAlignment> InlineStorage;
    FChunk InlineChunk;
    TNotNull<FChunk*> Head;
    TNotNull<FChunk*> Tail;
    int32 NumItems{};

public:
    // Inline chunk setup (mirrors the reset block at the end of Release()): the
    // metadata region starts at the embedded buffer, the data region is empty
    // (DataLowest == MemoryEnd). Head/Tail both point at the inline chunk.
    TScopedStructContainer()
        : InlineChunk(reinterpret_cast<Private::FItemMeta*>(&InlineStorage),
            reinterpret_cast<uint8*>(&InlineStorage) + InlineBytes,
            reinterpret_cast<uint8*>(&InlineStorage) + InlineBytes)
      , Head(MakeNotNull(&InlineChunk))
      , Tail(MakeNotNull(&InlineChunk))
    {
    }

    ~TScopedStructContainer() noexcept
    {
        Release();
    }

    // The pool holds pointers into its own embedded storage (InlineChunk, InlineStorage),
    // so a copy would alias the same chunk memory (double free on destruction) and a move
    // would leave the source referencing the target's inline storage. Call-scoped usage
    // never needs either.
    TScopedStructContainer(const TScopedStructContainer&)            = delete;
    TScopedStructContainer& operator=(const TScopedStructContainer&) = delete;
    TScopedStructContainer(TScopedStructContainer&&)                 = delete;
    TScopedStructContainer& operator=(TScopedStructContainer&&)      = delete;

    /**
     * In-place constructs a native struct via placement new; the returned not-null pointer
     * can be read/written directly (inout).
     * No InitializeStruct here: the C++ constructor IS the construction (unlike Add,
     * which has no compile-time type and must go through reflection). Running both would
     * double-construct. Destruction stays symmetric: DestroyStruct resolves to the same
     * C++ destructor for native structs.
     */
    template <CHasStaticStruct T, typename... TArgs>
    [[nodiscard]] TNotNull<T*> Emplace(TArgs&&... Args)
    {
        const UScriptStruct* ScriptStruct = T::StaticStruct();
        const auto Pointer                = Allocate(ScriptStruct, sizeof(T), alignof(T));
        return MakeNotNull(new(Pointer) T(Forward<TArgs>(Args)...));
    }

    /**
     * Reflection-initializes and copies from a view; covers runtime/BP structs.
     * Two steps, same as FInstancedStructContainer: InitializeStruct constructs a valid
     * default instance (zeroes trivial members, runs nested constructors), then
     * CopyScriptStruct copies property-wise. Copying onto raw memory without construction
     * would be UB for structs with non-trivial members (FString, TArray, ...) and would
     * break the later DestroyStruct.
     */
    [[nodiscard]] FStructView Add(const FConstStructView& View)
    {
        check(View.IsValid());
        const UScriptStruct* ScriptStruct = View.GetScriptStruct();
        const auto Pointer = Allocate(ScriptStruct, ScriptStruct->GetStructureSize(), ScriptStruct->GetMinAlignment());
        ScriptStruct->InitializeStruct(Pointer);
        if (const auto* Source = View.GetMemory())
        {
            ScriptStruct->CopyScriptStruct(Pointer, Source);
        }
        return FStructView(ScriptStruct, Pointer);
    }

    /** Index access; out-of-range check(). */
    FStructView operator[](const int32 Index)
    {
        return GetView<Enum::EConstness::Mutable>(Index);
    }

    FConstStructView operator[](const int32 Index) const
    {
        return GetView<Enum::EConstness::Const>(Index);
    }

    [[nodiscard]] int32 Num() const
    {
        return NumItems;
    }

    template <Enum::EConstness Constness>
    class TIteratorImpl
    {
    private:
        using FPoolType = std::conditional_t<Constness == Enum::EConstness::Const, const TScopedStructContainer,
                                             TScopedStructContainer>;
        using FViewType = std::conditional_t<Constness == Enum::EConstness::Const, FConstStructView, FStructView>;

        FPoolType* Pool{};
        int32 Index{};

    public:
        TIteratorImpl() = default;

        TIteratorImpl(FPoolType& Pool, const int32 Index)
            : Pool(&Pool)
          , Index(Index)
        {
        }

        [[nodiscard]] FViewType operator*() const
        {
            return (*Pool)[Index];
        }

        TIteratorImpl& operator++()
        {
            ++Index;
            return *this;
        }

        TIteratorImpl operator++(int)
        {
            TIteratorImpl Copy = *this;
            ++Index;
            return Copy;
        }

        bool operator==(const TIteratorImpl& Other) const
        {
            return Pool == Other.Pool && Index == Other.Index;
        }

        bool operator!=(const TIteratorImpl& Other) const
        {
            return !(*this == Other);
        }
    };

    using FTIterator      = TIteratorImpl<Enum::EConstness::Mutable>;
    using FTConstIterator = TIteratorImpl<Enum::EConstness::Const>;

    FTIterator begin()
    {
        return FTIterator(*this, 0);
    }

    FTIterator end()
    {
        return FTIterator(*this, NumItems);
    }

    FTConstIterator begin() const
    {
        return FTConstIterator(*this, 0);
    }

    FTConstIterator end() const
    {
        return FTConstIterator(*this, NumItems);
    }

private:
    // Linear walk over the chunk chain: O(#chunks). With the typical call-scoped profile
    // everything fits in the inline chunk (single chunk), so this is effectively O(1).
    // The method is const but can yield a mutable view; safe only because the public
    // entry points (const/non-const operator[]) pair the constness with the view type.
    template <Enum::EConstness Constness>
    [[nodiscard]] auto GetView(int32 Index) const
    {
        using FResultType = std::conditional_t<Constness == Enum::EConstness::Const, FConstStructView, FStructView>;

        check(Index >= 0 && Index < NumItems);
        for (const FChunk* Chunk = Head; Chunk != nullptr; Chunk = Chunk->Next)
        {
            if (Index < Chunk->MetadataNum)
            {
                const Private::FItemMeta& Item = Chunk->Metadata[Index];
                return FResultType(Item.ScriptStruct, Item.Memory);
            }
            Index -= Chunk->MetadataNum;
        }

        UE_UNREACHABLE_CODE(return FResultType();)
    }

    /** Always succeeds: opens a new chunk if needed, so the returned pointer is never null. */
    [[nodiscard]] TNotNull<uint8*> Allocate(const TNotNull<const UScriptStruct*> ScriptStruct, const int32 Size,
        const int32 Alignment)
    {
        // Fast path: the tail chunk has free space in the vast majority of calls
        // (append-only container, typical profile fits in the single inline chunk).
        if (auto* Pointer = TryAllocate(Tail, ScriptStruct, Size, Alignment))
        {
            return Pointer;
        }

        // Slow path: the tail chunk is full (metadata or data region exhausted).
        // Open a fresh heap chunk and append it to the chain. Existing elements are NOT
        // moved - the new chunk only receives future elements, so all views that were
        // issued before stay valid.
        const auto NewChunk = AllocateChunk(Size);
        NewChunk->Prev      = Tail;     // link back to the old tail
        Tail->Next          = NewChunk; // link forward from the old tail
        Tail                = NewChunk; // the new chunk becomes the tail

        // A fresh chunk is sized (AllocateChunk) so it can always hold this element;
        // the check is a safety net, not an expected failure path.
        auto* Pointer = TryAllocate(NewChunk, ScriptStruct, Size, Alignment);
        check(Pointer != nullptr);

        return Pointer;
    }

    /**
     * Tries to place a new element in the given chunk. Only the tail chunk can accept new
     * elements (append-only, older chunks are full and never revisited).
     * Invariant: the new metadata entry and the new data slot must not overlap, i.e. the
     * aligned data pointer must stay above the end of the metadata region (checked below).
     * Returns nullptr when the chunk is full; the caller then opens a new chunk.
     */
    [[nodiscard]] uint8* TryAllocate(const TNotNull<FChunk*> Chunk, const TNotNull<const UScriptStruct*> ScriptStruct,
        const int32 Size,
        const int32 Alignment)
    {
        // Metadata region: [Chunk->Metadata, MetadataEnd), one FItemMeta per element,
        // grows upward (bump). MetadataEnd = the slot where the new entry would go.
        // (TNotNull blocks pointer arithmetic on MSVC - deleted operator bool pollutes
        // built-in operator resolution - so convert to the raw pointer first.)
        Private::FItemMeta* Metadata = Chunk->Metadata;
        auto* MetadataEnd            = Metadata + Chunk->MetadataNum;

        // Data region: [DataLowest, MemoryEnd), grows downward (bump). The new slot is
        // placed at the lowest still-free address: DataLowest - Size, then aligned down
        // to the element's required alignment. AlignDown only lowers the address, so
        // the slot can never overlap data that was placed before (it sits below it).
        uint8* DataLowest = Chunk->DataLowest;
        auto* Pointer     = AlignDown(DataLowest - Size, Alignment);

        // Collision check: the new data slot [Pointer, Pointer+Size) must stay strictly
        // above the metadata region including the new entry [MetadataEnd, MetadataEnd+16).
        // If not, the chunk is full - return nullptr so the caller opens a new chunk.
        // (Alignment slack wasted by AlignDown is exactly what ChunkSlackBytes in
        // AllocateChunk accounts for.)
        if (Pointer < reinterpret_cast<uint8*>(MetadataEnd) + sizeof(Private::FItemMeta))
        {
            return nullptr;
        }

        // Construct the metadata entry in place (raw bump memory, so placement new is
        // the formally correct construction; the TNotNull members are null-checked
        // here in debug/development builds). The element data itself is NOT constructed
        // yet - Add()/Emplace() do that right after this function returns.
        new(&Metadata[Chunk->MetadataNum]) Private::FItemMeta{
            .ScriptStruct = ScriptStruct,
            .Memory       = Pointer
        };
        ++Chunk->MetadataNum;        // commit the metadata entry (bump up)
        Chunk->DataLowest = Pointer; // commit the data slot (bump down)
        ++NumItems;                  // container-wide element count
        return Pointer;
    }

    /**
     * Opens a new heap chunk. A chunk is ONE contiguous malloc block holding three
     * things (see FChunk):
     *
     *   [ FChunk header ][ metadata region ][ data region ]
     *   ^ base           ^ grows up        ^ grows down (from the block end)
     *
     * Everything lives in a single allocation, so Release() frees the whole chunk with
     * one FMemory::Free and elements are never allocated individually.
     * Called only when TryAllocate() failed on the current tail chunk (not enough room
     * for one more metadata entry + one data slot).
     */
    [[nodiscard]] TNotNull<FChunk*> AllocateChunk(const int32 NeededDataSize)
    {
        // Total bytes of the whole block, rounded up to BlockAlignment:
        //   sizeof(FChunk)               - the C++ header object itself
        //   sizeof(Private::FItemMeta)   - room for at least one metadata entry
        //                                  (the element that is about to be added)
        //   data region                  - at least OverflowChunkBytes, or more if the
        //                                  element itself is bigger than a whole chunk
        //   + ChunkSlackBytes            - alignment slack: AlignDown() in TryAllocate
        //                                  can lower the data pointer by up to
        //                                  Alignment-1 bytes, and alignments beyond 16
        //                                  (e.g. alignas(32)) need extra headroom
        // Align(..., BlockAlignment)     - the block end (MemoryEnd) must be 16-aligned
        //                                  so the first data slot can be 16-aligned too
        const int32 TotalBytes = Align(
            static_cast<int32>(sizeof(FChunk)) + static_cast<int32>(sizeof(Private::FItemMeta))
            + FMath::Max(OverflowChunkBytes, Align(NeededDataSize, BlockAlignment)) + ChunkSlackBytes,
            BlockAlignment);

        // One malloc for the whole block. The alignment argument guarantees the base
        // address is 16-aligned, so the header, the metadata entries and (after
        // AlignDown in TryAllocate) the data slots all sit on valid alignments.
        auto* Chunk = static_cast<FChunk*>(FMemory::Malloc(TotalBytes, BlockAlignment));

        // Placement-new the FChunk header into the raw malloc memory: FMemory::Malloc
        // does not run constructors. The constructor wraps the three pointers in
        // TNotNull (null-checked in debug/development builds):
        //   1st arg - metadata region start: right after the header, rounded up to
        //             BlockAlignment (sizeof(FChunk) happens to be a multiple of 16
        //             today, so this is a no-op; the Align is a safety net for future
        //             header layout changes)
        //   2nd/3rd - DataLowest and MemoryEnd both start at one-past-the-end: the data
        //             region is empty and will grow DOWNWARD from the block end as
        //             elements are placed via AlignDown in TryAllocate
        new(Chunk) FChunk(
            reinterpret_cast<Private::FItemMeta*>(Align(reinterpret_cast<uint8*>(Chunk + 1), BlockAlignment)),
            reinterpret_cast<uint8*>(Chunk) + TotalBytes, reinterpret_cast<uint8*>(Chunk) + TotalBytes);

        // A freshly allocated and initialized chunk is guaranteed non-null.
        return MakeNotNull(Chunk);
    }

    /**
     * Destroys all elements (reverse creation order, chunk by chunk) and frees every heap
     * chunk, restoring the container to its fresh state. Called by the destructor; safe to call
     * more than once.
     *
     * This is the "release everything" counterpart of a Reset() API (not provided in v1):
     * Reset() would destroy the elements but keep the allocated chunks for reuse, while
     * Release() always frees them.
     */
    void Release()
    {
        // Walk the chunk chain backwards (Prev links). Each chunk is visited exactly
        // once; heap chunks are freed on the way, the inline chunk is reset in place.
        // The walk variable is a RAW pointer: the chain terminates on nullptr, which
        // TNotNull cannot represent.
        FChunk* Chunk = Tail;
        while (Chunk != nullptr)
        {
            // Destroy this chunk's elements in REVERSE creation order (conservative
            // convention for struct destructors: a struct may reference another element
            // created after it). Metadata entries are in creation order, so iterate
            // them backwards. DestroyStruct is the reflection destructor - it matches
            // both construction paths: InitializeStruct (Add) and placement new
            // (Emplace).
            for (int32 Index = Chunk->MetadataNum - 1; Index >= 0; --Index)
            {
                const Private::FItemMeta& Item = Chunk->Metadata[Index];
                Item.ScriptStruct->DestroyStruct(Item.Memory);
            }
            Chunk->MetadataNum = 0; // all elements destroyed; metadata region is empty again

            // Unlink and free heap chunks. The inline chunk is a member of the
            // container object - it must NOT be freed, only its state is reset below.
            auto* Prev = Chunk->Prev;
            if (Chunk != &InlineChunk)
            {
                FMemory::Free(Chunk); // one Free per chunk: header + metadata + data were one malloc
            }
            Chunk = Prev;
        }

        // Reset the container to its fresh state (mirrors the constructor) so Release()
        // is idempotent and the inline chunk is ready for the next use.
        Head                    = MakeNotNull(&InlineChunk);
        Tail                    = MakeNotNull(&InlineChunk);
        InlineChunk.MetadataNum = 0;
        InlineChunk.DataLowest  = reinterpret_cast<uint8*>(&InlineStorage) + InlineBytes;
        InlineChunk.MemoryEnd   = InlineChunk.DataLowest;
        InlineChunk.Next        = nullptr; // no heap chunks remain
        InlineChunk.Prev        = nullptr;
        NumItems                = 0;
    }
};

/** Rem::Ranges interop; follows the GetNum convention of FInstancedStructContainer. */
template <int32 InlineBytes, int32 OverflowChunkBytes>
[[nodiscard]] int32 GetNum(const TScopedStructContainer<InlineBytes, OverflowChunkBytes>& Container)
{
    return Container.Num();
}
}
