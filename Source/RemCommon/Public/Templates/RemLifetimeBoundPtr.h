// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include "Misc/AssertionMacros.h"
#include "Templates/NonNullPointer.h"
#include "Templates/UnrealTemplate.h"

#include "Macro/RemGenerateMembersMacro.h"
#include "RemMisc.h"
#include "RemNotNull.h"

namespace Rem
{
/**
 * @brief Whether the liveness object can hand out the base address of the object it tracks.
 *
 * Satisfied by the liveness types Rem::IsValid accepts that also expose a payload: either a `Get()`
 * member (`TWeakObjectPtr`, `TObjectPtr`, `FWeakObjectPtr`, `TSharedPtr`) or a dereference operator
 * (raw pointer, `TNotNull`). A liveness that only answers `IsValid()` has no payload - it can still
 * drive the general storage, it just cannot drive the compact one.
 * @note A liveness can be strong: a `TSharedPtr` keeps the payload alive while the handle exists. Use
 * `TWeakPtr` when the handle must not extend the payload's lifetime - it has no payload, so it drives
 * the general storage only.
 */
template <typename TLiveness>
concept CHasLifetimePayload = requires(const TLiveness& Liveness)
                              {
                                  Liveness.Get();
                              }
                              || requires(const TLiveness& Liveness)
                              {
                                  std::addressof(*Liveness);
                              };

/**
 * @brief Base address of the object the liveness tracks, null when the liveness is invalid.
 * @note The accessor chain has to stay in sync with Rem::CHasLifetimePayload.
 */
template <CHasLifetimePayload TLiveness>
[[nodiscard]] auto GetLifetimePayloadPointer(const TLiveness& Liveness)
{
    if constexpr (requires { Liveness.Get(); })
    {
        return Liveness.Get();
    }
    else
    {
        return IsValid(Liveness)
                   ? std::addressof(*Liveness)
                   : nullptr;
    }
}

/** Pointer to the payload of the liveness; only well-formed when Rem::CHasLifetimePayload holds. */
template <typename TLiveness>
using TLifetimePayloadPointer = decltype(GetLifetimePayloadPointer(std::declval<const TLiveness&>()));

/** Type of the payload of the liveness, i.e. the object type a pointee can be derived from. */
template <typename TLiveness>
using TLifetimePayload = std::remove_pointer_t<TLifetimePayloadPointer<TLiveness>>;

namespace Private
{
/**
 * @brief Payload pointer of a liveness, or `void` when the liveness has none (a validity-only liveness).
 * @note Going through a trait keeps the payload types out of member declarations that must be
 * instantiable for every liveness Rem::IsValid accepts.
 */
template <typename TLiveness, typename = void>
struct TLifetimePayloadOf
{
    using PointerType = void;
};

template <typename TLiveness>
struct TLifetimePayloadOf<TLiveness, std::void_t<TLifetimePayloadPointer<TLiveness>>>
{
    using PointerType = TLifetimePayloadPointer<TLiveness>;
};

/**
 * @brief The pointee is the liveness payload itself or a base of it, so the pointer is produced by a
 * `static_cast` that applies a compile-time offset.
 * @note The convertibility branch keeps the cast well-formed: it rejects a payload that would drop
 * constness and a payload whose base is ambiguous (a repeated non-virtual base).
 */
template <typename TLiveness, typename TPointee, typename = void>
struct TIsCompactLifetimeBoundPointee : std::false_type
{
};

template <typename TLiveness, typename TPointee>
struct TIsCompactLifetimeBoundPointee<TLiveness, TPointee, std::void_t<TLifetimePayloadPointer<TLiveness>>>
    : std::bool_constant<std::is_base_of_v<std::remove_cv_t<TPointee>, std::remove_cv_t<TLifetimePayload<TLiveness>>>
                         && std::is_convertible_v<TLifetimePayloadPointer<TLiveness>, TPointee*>>
{
};

/** General storage: the pointer's object may be a different object than the one the liveness tracks. */
template <typename TLiveness, typename TPointee>
struct TLifetimeBoundPtrStorage
{
    TLiveness Liveness{};

    /** @note The uninitialized state is the not-bound state; it is only observable while IsValid() is false. */
    TNonNullPtr<TPointee> Pointer{EDefaultConstructNonNullPtr::Uninitialized};

    TLifetimeBoundPtrStorage() = default;

    /** @note Tolerates an invalid pointer so the type-erasure conversion can carry a dead liveness over. */
    TLifetimeBoundPtrStorage(TLiveness InLiveness, TPointee* InPointer);
};

/** Compact storage: the pointee is derived from the liveness payload, so only the liveness is stored. */
template <typename TLiveness, typename TPointee>
struct TLifetimeBoundPtrCompactStorage
{
    TLiveness Liveness{};

    TLifetimeBoundPtrCompactStorage() = default;

    explicit TLifetimeBoundPtrCompactStorage(TLiveness InLiveness);
};

template <typename TLiveness, typename TPointee>
using TLifetimeBoundPtrStorageT = std::conditional_t<TIsCompactLifetimeBoundPointee<TLiveness, TPointee>::value,
                                                     TLifetimeBoundPtrCompactStorage<TLiveness, TPointee>,
                                                     TLifetimeBoundPtrStorage<TLiveness, TPointee>>;
}

/**
 * @brief A pointer whose lifetime is bound to a separate liveness object.
 *
 * @tparam TLiveness Decides validity (judged by `Rem::IsValid`) and, for the compact storage, hands
 *         out the base address the pointee is derived from.
 * @tparam TPointee  Type the pointer refers to.
 *
 * The pointer half takes one of two storages, decided per instantiation rather than by a second type:
 * - **General**: the pointee has no inheritance relation to `Rem::TLifetimePayload<TLiveness>` (an
 *   actor's worker component, a data table row), so the pointer is stored. Its object may be a
 *   different object than the one the liveness tracks.
 * - **Compact**: the pointee is the liveness payload itself or a base of it, so the pointer is derived
 *   from the payload by a compile-time offset and nothing extra is stored.
 *
 * Invariants: `IsValid()` implies the stored pointer is initialized. The reverse does not hold - the
 * liveness may die while the pointer is still stored - so the pointer is only ever read through
 * `Get()` / `TryGet()` / `operator*` / `operator->`, which all consult the liveness first: asserted
 * in `Get()`, branched in `TryGet()`.
 *
 * @note Runtime, non-serialized reference: not UPROPERTY-compatible, no GC reference, no path. A
 * reference that has to survive serialization or an editor recompile is a `TSoftObjectPtr`.
 * @see TWeakInterfacePtr - the engine's fixed-shape (`TWeakObjectPtr<UObject>` plus an interface
 * pointer) equivalent.
 */
template <typename TLiveness, typename TPointee>
struct TLifetimeBoundPtr : Private::TLifetimeBoundPtrStorageT<TLiveness, TPointee>
{
    using ElementType  = TPointee;
    using LivenessType = TLiveness;

    /** Whether the pointee is derived from the liveness payload instead of being stored (compact storage). */
    static constexpr bool bCompactStorage = Private::TIsCompactLifetimeBoundPointee<TLiveness, TPointee>::value;

private:
    using Super = Private::TLifetimeBoundPtrStorageT<TLiveness, TPointee>;

public:
    REM_DEFINE_THE_RULE_OF_FIVE(TLifetimeBoundPtr);

    /** Not bound: no liveness, no pointer. */
    TLifetimeBoundPtr() = default;

    /**
     * @brief Binds a liveness to the pointer it tracks (general storage only).
     * @param InLiveness The object whose lifetime bounds this pointer.
     * @param InPointer The pointee; its object may differ from the object @p InLiveness tracks.
     */
    TLifetimeBoundPtr(TLiveness InLiveness, TNotNull<TPointee*> InPointer)
        requires (!bCompactStorage);

    /**
     * @brief Binds a liveness whose payload the pointee is derived from (compact storage only).
     * @param InLiveness The object whose lifetime bounds this pointer and whose payload the pointee is.
     */
    explicit TLifetimeBoundPtr(TLiveness InLiveness)
        requires (bCompactStorage);

    /**
     * @brief Type-erasure bridge: same pointee, wider liveness, e.g. a concrete type created where it
     * is known (`TWeakObjectPtr<UMyComponent>`) consumed behind an interface (`TWeakObjectPtr<UObject>`).
     * @note Widening the liveness erases the type the compact storage would derive the pointee from, so
     * only the general storage can be the target; the bound state (including a dead liveness) is carried
     * over. The type checks live in the definition body instead of a requires-clause because MSVC cannot
     * match an out-of-line member template definition whose constraint uses the enclosing class's
     * template parameters (C2244).
     */
    template <typename TOtherLiveness, typename TOtherPointee>
    TLifetimeBoundPtr(const TLifetimeBoundPtr<TOtherLiveness, TOtherPointee>& Other);

    /** The object whose lifetime bounds this pointer. */
    [[nodiscard]] const TLiveness& GetLiveness() const;

    /**
     * @brief The object the liveness tracks; the identity behind the comparison operators.
     * @return Null when the liveness is invalid.
     */
    [[nodiscard]] Private::TLifetimePayloadOf<TLiveness>::PointerType GetObject() const
        requires CHasLifetimePayload<TLiveness>;

    /** Whether the liveness is still valid. */
    [[nodiscard]] bool IsValid() const;

    /**
     * @brief The pointee, asserted non-null.
     * @pre `IsValid()`
     * @note The primitive accessor: it pays the assertion only, so the valid path carries no validity
     * branch to skip. A violated precondition reads the payload the dead liveness no longer guarantees.
     */
    [[nodiscard]] TNotNull<TPointee*> Get() const;

    /**
     * @brief The pointee, or null when the liveness is invalid.
     * @note The nullable accessor: it adds the validity branch and delegates to `Get()`, so the
     * stored pointer is never read in the invalid state.
     */
    [[nodiscard]] TPointee* TryGet() const;

    /** Unbinds: clears the liveness and the pointer together, never half of the pair. */
    void Reset();

    [[nodiscard]] bool operator==(const TLifetimeBoundPtr& Other) const;
    [[nodiscard]] bool operator!=(const TLifetimeBoundPtr& Other) const;

    /**
     * @brief The pointee, asserted valid and non-null.
     * @pre `IsValid()`
     * @note The assertion lives in `Get()`; this only forwards.
     */
    [[nodiscard]] TPointee& operator*() const;

    /**
     * @brief Member access on the pointee, asserted valid and non-null.
     * @pre `IsValid()`
     * @note The assertion lives in `Get()`; this only forwards.
     */
    [[nodiscard]] TPointee* operator->() const;
};

template <typename TLiveness, typename TPointee>
Private::TLifetimeBoundPtrStorage<TLiveness, TPointee>::TLifetimeBoundPtrStorage(TLiveness InLiveness,
    TPointee* InPointer)
    : Liveness{MoveTemp(InLiveness)}
  , Pointer{InPointer
                ? TNonNullPtr<TPointee>{InPointer}
                : TNonNullPtr<TPointee>{EDefaultConstructNonNullPtr::Uninitialized}
    }
{
}

template <typename TLiveness, typename TPointee>
Private::TLifetimeBoundPtrCompactStorage<TLiveness, TPointee>::TLifetimeBoundPtrCompactStorage(TLiveness InLiveness)
    : Liveness{MoveTemp(InLiveness)}
{
}

template <typename TLiveness, typename TPointee>
TLifetimeBoundPtr<TLiveness, TPointee>::TLifetimeBoundPtr(TLiveness InLiveness, TNotNull<TPointee*> InPointer)
    requires (!bCompactStorage)
    : Super{MoveTemp(InLiveness), InPointer}
{
}

template <typename TLiveness, typename TPointee>
TLifetimeBoundPtr<TLiveness, TPointee>::TLifetimeBoundPtr(TLiveness InLiveness)
    requires (bCompactStorage)
    : Super{MoveTemp(InLiveness)}
{
}

template <typename TLiveness, typename TPointee>
template <typename TOtherLiveness, typename TOtherPointee>
TLifetimeBoundPtr<TLiveness, TPointee>::TLifetimeBoundPtr(const TLifetimeBoundPtr<TOtherLiveness, TOtherPointee>& Other)
    : Super{Other.GetLiveness(), Other.TryGet()}
{
    static_assert(!bCompactStorage,
            "only the general storage can be the target of the erasure bridge: widening the liveness erases the type the compact storage derives the pointee from")
        ;
    static_assert(std::same_as<TOtherPointee, TPointee>,
        "the erasure bridge widens the liveness, it does not change the pointee");
    static_assert(std::convertible_to<TOtherLiveness, TLiveness>,
        "the target liveness has to be constructible from the source liveness");
}

template <typename TLiveness, typename TPointee>
const TLiveness& TLifetimeBoundPtr<TLiveness, TPointee>::GetLiveness() const
{
    return Super::Liveness;
}

template <typename TLiveness, typename TPointee>
Private::TLifetimePayloadOf<TLiveness>::PointerType TLifetimeBoundPtr<TLiveness, TPointee>::GetObject() const
    requires CHasLifetimePayload<TLiveness>
{
    return GetLifetimePayloadPointer(Super::Liveness);
}

template <typename TLiveness, typename TPointee>
bool TLifetimeBoundPtr<TLiveness, TPointee>::IsValid() const
{
    return Rem::IsValid(Super::Liveness);
}

template <typename TLiveness, typename TPointee>
TNotNull<TPointee*> TLifetimeBoundPtr<TLiveness, TPointee>::Get() const
{
    check(IsValid());

    if constexpr (bCompactStorage)
    {
        return static_cast<TPointee*>(GetLifetimePayloadPointer(Super::Liveness));
    }
    else
    {
        return Super::Pointer.Get();
    }
}

template <typename TLiveness, typename TPointee>
TPointee* TLifetimeBoundPtr<TLiveness, TPointee>::TryGet() const
{
    if (!IsValid())
    {
        return nullptr;
    }

    return Get();
}

template <typename TLiveness, typename TPointee>
void TLifetimeBoundPtr<TLiveness, TPointee>::Reset()
{
    *this = TLifetimeBoundPtr{};
}

template <typename TLiveness, typename TPointee>
bool TLifetimeBoundPtr<TLiveness, TPointee>::operator==(const TLifetimeBoundPtr& Other) const
{
    return TryGet() == Other.TryGet();
}

template <typename TLiveness, typename TPointee>
bool TLifetimeBoundPtr<TLiveness, TPointee>::operator!=(const TLifetimeBoundPtr& Other) const
{
    return !(*this == Other);
}

template <typename TLiveness, typename TPointee>
TPointee& TLifetimeBoundPtr<TLiveness, TPointee>::operator*() const
{
    return *Get();
}

template <typename TLiveness, typename TPointee>
TPointee* TLifetimeBoundPtr<TLiveness, TPointee>::operator->() const
{
    return Get();
}
}
