// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Templates/Casts.h"
#include "Templates/Function.h"
#include "Templates/RemLifetimeBoundPtr.h"
#include "Templates/RemTypeString.h"
#include "UObject/Class.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

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
 * A family that hands out something else specializes this template and declares its own `FHandle`,
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
            return FTyped{Liveness, MakeNotNull(static_cast<TFamily::FPointee*>(Erased.TryGet()))};
        }
    }
};

// ReSharper disable once CommentTypo
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
 *     MYMODULE_API static Rem::TFeatureRegistry<FMyFamily> Instance; // the owning module's export macro
 * };
 * ```
 *
 * A family and its module then:
 * - declare the family's registry on the family type and the two `extern template class` declarations
 *   (undecorated) in the family's header - after the family's `TFeatureHandleTraits` specialization, if it
 *   has one (the note below says why);
 * - define `Instance` and emit the family's instantiations in the family's `.cpp`
 *   (`template class MYMODULE_API Rem::TFeatureRegistry<FMyFamily>;` and the same for `TFeatureRegistration`);
 * - publish a consumer surface for the family (its factory / typed query) and hand
 *   `Rem::TFeatureRegistration<TFamily>` to the modules that provide a worker.
 *
 * A consumer asks that published surface, never the module behind it: a providing module can be unloaded
 * at any moment, the family's registry cannot.
 *
 * The request is an **input**: the mechanism passes it by `const FRequest&` end to end, so a
 * contribution sees the caller's object instead of a copy - and must not retain the reference. The
 * mechanism treats the request as opaque: it never stores, compares or inspects it, so the family
 * decides its shape. A request should name what is being asked for, with a payload behind it
 * rather than in it.
 *
 * @note The registry lives in the family's own *non-template* type: a template's static data member
 * exists once per module, so a contribution registered in one DLL would be invisible in another.
 * @note A handle that is invalid means "this contribution has nothing", so the walk simply continues -
 * "I do not apply" and "I have nothing" are the same answer.
 * @note The chain keeps registration order across removals: the first registered contribution that
 * answers wins, and a removal must not change which survivor that is.
 * @note The registry is not synchronized: a registration is made at module load, a query on the game
 * thread, and the walk is not safe against a concurrent registration.
 * @note **The mechanism's one structural bet is the key**: a contribution is keyed by the owner type it
 * answers for (`UObject` = the wildcard). A family that needs a different key changes this mechanism -
 * `TFeatureHandleTraits` carries the handle shape, not the key - rather than extending it from outside.
 * That change is deferred until a family actually needs it: every family today keys by a `UClass`, so a
 * second key shape would have no user to validate it.
 * @note **Any module can own a family: the class carries no export macro.** The export sits on the
 * family's instantiations instead. The module that owns the family declares `Instance` on the family type
 * with its own `_API` macro - the one object every consumer shares - defines it and emits the family's two
 * instantiations in the family's `.cpp`, and the family's header declares those instantiations
 * `extern template class`. The declaration is undecorated on purpose: it only says "do not instantiate
 * locally", and the symbols come from the exporting module. A `_API` on the class would have made every
 * consumer's instantiation `dllimport` and forced the mechanism's module to export every family - the
 * missing `Provide` / destructor symbols of the LNK2019 this form removes. Because the walk's members are
 * defined in the class body, a compiler may still instantiate them in a consumer's module: only `Instance`
 * and the class layout stay a cross-DLL contract. The exported instantiations are insurance for a future
 * member that is not defined in the class: today every member is, so the symbols a consumer imports can be
 * as small as the family's `Instance`.
 * @note A family that specializes `TFeatureHandleTraits` declares that specialization before its
 * `extern template` declarations: seeing an `extern template` instantiates the registry on MSVC, so the
 * registry's handle type has to come from the specialization rather than from the primary template.
 */
template <typename TFamily>
class TFeatureRegistry
{
public:
    using FRequest = TFamily::FRequest;
    using FHandle  = TFeatureHandleTraits<TFamily>::FHandle;

    template <typename TLiveness>
    using TTypedHandle = TFeatureHandleTraits<TFamily>::template TTypedHandle<TLiveness>;

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
            if (const UClass* const ContributionOwner = Entry.Contribution.OwnerType;
                OwnerType && ContributionOwner != Wildcard && ContributionOwner != OwnerType)
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
     * @note For a consumer that knows the owner's type - a module that declares the handle's liveness
     * itself. The family's specs are its only consumer today, so the typed surface stays as it is
     * instead of growing an unproven caller's shape.
     */
    template <typename TLiveness>
    [[nodiscard]] TTypedHandle<TLiveness> ProvideTyped(const FRequest& Request) const
    {
        return TFeatureHandleTraits<TFamily>::template DeriveTypedHandle<TLiveness>(
            Provide(Request, TLiveness::StaticClass()));
    }

    /**
     * @brief Registers a contribution and returns its id.
     * @note A second contribution for an owner type that already has one is reported in development
     * only: the walk's winner is decided by registration order, so it would otherwise stay invisible.
     * @note An id is never zero: zero means "not registered".
     */
    [[nodiscard]] uint32 Add(FContribution Contribution)
    {
#if REM_WITH_DEVELOPMENT_ONLY_CODE

        const UClass* const NewOwner = Contribution.OwnerType;
        const bool bAlreadyClaimed   = Entries.ContainsByPredicate([NewOwner](const FEntry& Entry)
        {
            return static_cast<const UClass*>(Entry.Contribution.OwnerType) == NewOwner;
        });

        if (bAlreadyClaimed)
        {
            REM_LOG_FUNCTION(LogRemCommon, Warning,
                "a second contribution registers for {0} ({1}): the first registered one is asked first and stays the winner",
                NewOwner->GetPathName(), Rem::GetTypeName<TFamily>());
        }

#endif

        const uint32 Id = NextId++;
        Entries.Add(FEntry{Id, MoveTemp(Contribution)});
        return Id;
    }

    // ReSharper disable once GrammarMistakeInComment
    /**
     * @brief Removes the contribution with @p Id, if it is still registered.
     * @note The removal is order-preserving on purpose: the walk's outcome is registration order,
     * and a swap-removal would promote a later contribution over an earlier survivor.
     */
    void Remove(const uint32 Id)
    {
        Entries.RemoveAll([Id](const FEntry& Entry)
        {
            return Entry.Id == Id;
        });
    }

private:
    struct FEntry
    {
        uint32 Id{0};
        FContribution Contribution;
    };

    /** The registered contributions, kept in registration order. */
    TArray<FEntry> Entries;

    uint32 NextId{1};
};

/**
 * @brief Registers a contribution in @p TFamily's registry for as long as the object lives.
 *
 * The providing module owns one - its constructor registers and its destructor clears, so the module
 * cannot forget the clearing half, and the family never has to guess whether a registered contribution
 * is still backed by a loaded module. A contribution declares the owner type its workers belong to and
 * the function that hands one out (`UObject` is the wildcard; an invalid handle means "I have nothing",
 * and the walk continues):
 *
 * ```cpp
 * class FMyModule final : public IModuleInterface
 * {
 *     Rem::TFeatureRegistration<FMyFamily> Registration{
 *         {
 *             AActor::StaticClass(), // the owner type its workers belong to; UObject = the wildcard
 *             [](const FMyFamily::FRequest& Request) { return MakeHandleFor(Request); },
 *         },
 *     };
 * };
 * ```
 *
 * @note Registering from a module's file scope is safe in a *dependent* module: nothing runs the
 * contribution until a request arrives, and the family's own registry is initialized before any of
 * its dependents (the loader initializes a DLL's dependencies first). A file-scope registration in
 * the family's *own* module is not safe - static destruction order is unspecified, so it can be
 * destroyed after the registry's static storage, and its destructor would then touch a dead registry.
 * The family's module object owns the registration instead: its destructor runs while the module is
 * still loaded.
 */
template <typename TFamily>
class TFeatureRegistration
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
        if (Id == 0)
        {
            return;
        }

        TFamily::Instance.Remove(Id);
        Id = 0;
    }

    TFeatureRegistration(const TFeatureRegistration&)            = delete;
    TFeatureRegistration& operator=(const TFeatureRegistration&) = delete;
    TFeatureRegistration(TFeatureRegistration&&)                 = delete;
    TFeatureRegistration& operator=(TFeatureRegistration&&)      = delete;

private:
    uint32 Id{0};
};
}
