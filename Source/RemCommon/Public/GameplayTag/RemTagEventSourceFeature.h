// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h"

#include "Templates/RemFeatureRegistry.h"

class AActor;

namespace Rem::GameplayTag
{
/**
 * @brief The tag event source family: what the generic mechanism is told, plus the family's registry.
 *
 * The request is the actor a source is wanted for, a contribution answers for the owner type its workers
 * belong to (`UObject` is the wildcard), and the handle shape is the default one - an erased liveness
 * plus the tag event source interface, with `TryGetTagEventSource<TLiveness>` for the typed form.
 *
 * The request is a pointer by choice: the query names an existing object, so nothing is copied and a
 * contribution uses it directly. The mechanism takes the request as opaque and does not require a
 * pointer - a family whose query needs more context carries it in its request type.
 *
 * @note Keeping the registry in this non-template type is what makes one registration visible to every
 * module.
 * @see `Rem::TFeatureRegistry` (Templates/RemFeatureRegistry.h) for the mechanism - what a contribution
 * declares and how a providing module registers one - and `IRemGetLifetimeBoundOwnerInterface`
 * (Interface/RemGetLifetimeBoundOwnerInterface.h) plus its typed accessor
 * `Rem::TGetLifetimeBoundObjectPtr` (Interface/RemGetLifetimeBoundObjectPtrInterface.h) for the identity
 * protocol a worker declares its owner with.
 */
struct FTagEventSourceFamily
{
    using FRequest = TNotNull<AActor*>;
    using FPointee = IRemTagEventSourceInterface;

    /** The family's registry: declared here, defined once in the family's `.cpp`. */
    REMCOMMON_API static TFeatureRegistry<FTagEventSourceFamily> Instance;
};

/** The family's public surface, spelled once so a call site reads after the family. */
struct FTagEventSourceFeature
{
    using FStorage = TFeatureRegistry<FTagEventSourceFamily>;

    using FContribution = FStorage::FContribution;
    using FRequest      = FStorage::FRequest;
    using FHandle       = FStorage::FHandle;

    template <typename TLiveness>
    using TTypedHandle = FStorage::TTypedHandle<TLiveness>;
};

/**
 * @brief Registers a contribution to this family for as long as the object lives; `Reset()` clears it
 * early.
 * @note A contribution is the owner type its workers belong to plus the function that hands one out,
 * and the providing module owns the registration - see `Rem::TFeatureRegistry` for both.
 */
using FTagEventSourceRegistration = TFeatureRegistration<FTagEventSourceFamily>;

// the registry's code lives in RemCommon: the class is exported and instantiated explicitly in
// the family's .cpp, so a caller imports the walk instead of carrying a copy of it


namespace Private
{
/**
 * @brief The source the actor itself provides through `IRemGetTagEventSourceInterface`, if any.
 * @note The family's intrinsic path: it is consulted before any contribution, and it needs no
 * registration because the actor is right there.
 */
[[nodiscard]] REMCOMMON_API FTagEventSourceFeature::FHandle TryGetActorProvidedSource(
    const FTagEventSourceFeature::FRequest& Actor);

/** The family's typed derivation, as the handle traits define it. */
template <typename TLiveness>
[[nodiscard]] FTagEventSourceFeature::TTypedHandle<TLiveness> DeriveTypedHandle(
    const FTagEventSourceFeature::FHandle& Erased)
{
    return TFeatureHandleTraits<FTagEventSourceFamily>::DeriveTypedHandle<TLiveness>(Erased);
}
}

/**
 * @brief The tag event source for @p Actor, bound to a concrete owner (liveness) type.
 *
 * @p TLiveness names the owner, not the worker: a worker that is its own owner is queried by its own
 * type and gets the compact handle, while a worker owned by another object (a component whose owner is
 * the actor) is normally asked for through the erased form - `TryGetLifetimeBoundInterfacePtr`
 * (GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h) - which carries that owner automatically.
 *
 * @note Asks the actor's getter first, then the contributions registered for @p TLiveness or for the
 * `UObject` wildcard.
 */
template <typename TLiveness>
[[nodiscard]] FTagEventSourceFeature::TTypedHandle<TLiveness> TryGetTagEventSource(
    const FTagEventSourceFeature::FRequest& Actor)
{
    if (const FTagEventSourceFeature::FHandle FromActor = Private::TryGetActorProvidedSource(Actor);
        FromActor.IsValid())
    {
        return Private::DeriveTypedHandle<TLiveness>(FromActor);
    }

    return FTagEventSourceFamily::Instance.ProvideTyped<TLiveness>(Actor);
}
}
