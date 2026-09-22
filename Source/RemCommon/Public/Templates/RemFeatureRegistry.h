// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include <type_traits>
#include <utility>

#include "CoreMinimal.h"
#include "Templates/Casts.h"
#include "Templates/Function.h"
#include "Templates/RemLifetimeBoundPtr.h"
#include "UObject/Class.h"
#include "UObject/WeakObjectPtr.h"

#include "Macro/RemAssertionMacros.h"
#include "Macro/RemLogMacros.h"
#include "RemCommonLog.h"
#include "RemNotNull.h"

namespace Rem
{
/**
 * @brief The default handle shape for a family whose workers are handed out as a lifetime-bound
 * interface pointer: an erased liveness plus the family's pointee, and a typed form per owner type.
 *
 * A family that hands out something else specialises this template and declares its own `FHandle`,
 * `TTypedHandle<TLiveness>` and `DeriveTypedHandle<TLiveness>(const FHandle&)`.
 *
 * @tparam TFamily declares `FPointee`, the interface its workers implement.
 */
template <typename TFamily>
struct TFeatureHandleTraits
{
    using FHandle = TLifetimeBoundPtr<TWeakObjectPtr<UObject>, typename TFamily::FPointee>;

    template <typename TLiveness>
    using TTypedHandle = TLifetimeBoundPtr<TWeakObjectPtr<TLiveness>, typename TFamily::FPointee>;

    /**
     * @brief Rebuilds the typed handle from the family's erased one.
     * @note The erased handle's pointee is the worker while the typed handle's liveness is the owner type
     * the caller named. When the typed handle is compact (the owner itself implements the interface) its
     * pointee is derived from the payload, so the named type is a promise - checked in development.
     */
    template <typename TLiveness>
    [[nodiscard]] static TTypedHandle<TLiveness> DeriveTypedHandle(const FHandle& Erased)
    {
        using FTyped = TTypedHandle<TLiveness>;

        if (!Erased.IsValid())
        {
            return {};
        }

        const TWeakObjectPtr<TLiveness> Liveness{Cast<TLiveness>(Erased.GetObject())};

        if constexpr (FTyped::bCompactStorage)
        {
            FTyped Typed{Liveness};

#if REM_WITH_DEVELOPMENT_ONLY_CODE

            // the owner type the caller named has to be the worker itself, or the derived pointee differs
            RemCheckCondition(static_cast<typename TFamily::FPointee*>(Typed.TryGet()) == Erased.TryGet());

#endif

            return Typed;
        }
        else
        {
            return FTyped{Liveness, MakeNotNull(static_cast<typename TFamily::FPointee*>(Erased.TryGet()))};
        }
    }
};

/**
 * @brief A family's registered contributions: the chain a family walks when its own path has nothing.
 *
 * A family describes itself with a traits type - its own name is the template parameter - that declares
 * what a contribution is asked for and what it hands back:
 *
 * ```cpp
 * struct FMyFamily
 * {
 *     using FRequest = TNotNull<AActor*>;
 *     using FPointee = IMyWorkerInterface;
 *
 *     REMCOMMON_API static Rem::TFeatureRegistry<FMyFamily> Instance;
 * };
 * ```
 *
 * @note The registry lives in the family's own *non-template* type: a template's static data member
 * exists once per module, so a contribution registered in one DLL would be invisible in another.
 * @note A handle that is invalid means "this contribution has nothing", so the walk simply continues -
 * "I do not apply" and "I have nothing" are the same answer.
 * @note The chain keeps registration order across removals: the first registered contribution that
 * answers wins, and a removal must not change which survivor that is.
 * @note **The mechanism's one structural bet is the key**: a contribution is keyed by the owner type it
 * answers for (`UObject` = the wildcard). A family that needs another key extends the traits (a `TKey`
 * plus a way to derive it from the request) rather than bending this one - that axis has a single user
 * today, so generalising it is cheap now and expensive later.
 * @note The class is exported and every family instantiates it explicitly in its own `.cpp`
 * (`template class TFeatureRegistry<FMyFamily>;`): the walk's code then lives in the owning module and
 * the callers import it, so only the class layout stays a cross-DLL contract.
 */
template <typename TFamily>
class REMCOMMON_API TFeatureRegistry
{
public:
    using FRequest = typename TFamily::FRequest;
    using FHandle  = typename TFeatureHandleTraits<TFamily>::FHandle;

    template <typename TLiveness>
    using TTypedHandle = typename TFeatureHandleTraits<TFamily>::template TTypedHandle<TLiveness>;

    /** One module's contribution to the family. */
    struct FContribution
    {
        /**
         * The owner (liveness) type this contribution answers for. A typed query only consults the
         * contributions registered for its own type plus the `UObject` ones, which are the wildcard.
         */
        TNotNull<UClass*> OwnerType;

        /**
         * The handle for the request, or an invalid one when this contribution has nothing.
         * @note The request is valid for the call only; a contribution that has to keep it copies
         * it into a member of its own.
         */
        TFunction<FHandle(const FRequest&)> Provide;
    };

    /**
     * @brief The first contribution registered for the request that yields a valid handle.
     * @note The unfiltered walk: every contribution is consulted, whatever owner type it answers for.
     * The owner-type filter is the overload below.
     */
    [[nodiscard]] FHandle Provide(const FRequest& Request) const
    {
        return Provide(Request, nullptr);
    }

    /**
     * @brief The walk with an owner-type filter; `nullptr` means "every contribution".
     * @note Public so a typed query can be a header-only wrapper over this one function instead of
     * reaching into the registry's storage from every caller.
     */
    [[nodiscard]] FHandle Provide(const FRequest& Request, const UClass* OwnerType) const
    {
        const UClass* const Wildcard = UObject::StaticClass();

        for (const FEntry& Entry : Entries)
        {
            const UClass* const ContributionOwner = Entry.Contribution.OwnerType;

            if (OwnerType && ContributionOwner != Wildcard && ContributionOwner != OwnerType)
            {
                continue;
            }

            if (!Entry.Contribution.Provide)
            {
                continue;
            }

            // an entry that applies may still have nothing to give: an invalid handle makes the walk go on
            if (const FHandle Provided = Entry.Contribution.Provide(Request);
                Provided.IsValid())
            {
                return Provided;
            }
        }

        return {};
    }

    /**
     * @brief The typed form of `Provide`: only the contributions registered for @p TLiveness, or for the
     * `UObject` wildcard.
     */
    template <typename TLiveness>
    [[nodiscard]] TTypedHandle<TLiveness> ProvideTyped(const FRequest& Request) const
    {
        return TFeatureHandleTraits<TFamily>::template DeriveTypedHandle<TLiveness>(
            Provide(Request, TLiveness::StaticClass()));
    }

    /** Registers a contribution and returns its id. */
    int32 Add(FContribution Contribution)
    {
#if REM_WITH_DEVELOPMENT_ONLY_CODE

        // the walk's outcome is decided by registration order, so a second contribution for an owner
        // type that already has one is reported - otherwise the winner is invisible
        const UClass* const NewOwner = Contribution.OwnerType;
        const bool bAlreadyClaimed   = Entries.ContainsByPredicate([NewOwner](const FEntry& Entry)
        {
            return static_cast<const UClass*>(Entry.Contribution.OwnerType) == NewOwner;
        });

        if (bAlreadyClaimed)
        {
            REM_LOG_FUNCTION(LogRemCommon, Warning,
                "a second contribution registers for {0}: the first registered one is asked first",
                NewOwner->GetName());
        }

#endif

        const int32 Id = NextId++;
        Entries.Add(FEntry{Id, MoveTemp(Contribution)});
        return Id;
    }

    /**
     * @brief Removes the contribution with @p Id, if it is still registered.
     * @note The removal is order-preserving on purpose: the walk's outcome is registration order,
     * and a swap-removal would promote a later contribution over an earlier survivor.
     */
    void Remove(const int32 Id)
    {
        Entries.RemoveAll([Id](const FEntry& Entry)
        {
            return Entry.Id == Id;
        });
    }

private:
    struct FEntry
    {
        int32 Id{INDEX_NONE};
        FContribution Contribution;
    };

    /** The registered contributions, kept in registration order. */
    TArray<FEntry> Entries;

    int32 NextId{1};
};

/**
 * @brief Registers a contribution in @p TFamily's registry for as long as the object lives.
 *
 * The providing module owns one - its constructor registers and its destructor clears, so the module
 * cannot forget the clearing half, and the family never has to guess whether a registered feature is
 * still backed by a loaded module.
 *
 * @note Registering from a module's file scope is safe: nothing runs the contribution until a request
 * arrives, and the family's own registry is initialized before any of its dependents (the loader
 * initializes a DLL's dependencies first).
 */
template <typename TFamily>
class REMCOMMON_API TFeatureRegistration
{
public:
    TFeatureRegistration() = default;

    explicit TFeatureRegistration(TFeatureRegistry<TFamily>::FContribution Contribution)
        : Id(TFamily::Instance.Add(MoveTemp(Contribution)))
    {
    }

    ~TFeatureRegistration()
    {
        Reset();
    }

    /** Clears the registration now; a second call, or the destructor after it, does nothing. */
    void Reset()
    {
        if (Id == INDEX_NONE)
        {
            return;
        }

        TFamily::Instance.Remove(Id);
        Id = INDEX_NONE;
    }

    TFeatureRegistration(const TFeatureRegistration&)            = delete;
    TFeatureRegistration& operator=(const TFeatureRegistration&) = delete;

private:
    int32 Id{INDEX_NONE};
};
} // namespace Rem
