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
 * @note Keeping the registry in this non-template type is what makes one registration visible to every
 * module; see Rem::TFeatureRegistry for the rest of the contract.
 */
struct FTagEventSourceFamily
{
    using FRequest = TNotNull<AActor*>;
    using FPointee = IRemTagEventSourceInterface;

    /** The family's registry: declared here, defined once in the family's `.cpp`. */
    REMCOMMON_API static Rem::TFeatureRegistry<FTagEventSourceFamily> Instance;
};

/** The family's public surface, spelled once so a call site reads after the family. */
struct FTagEventSourceFeature
{
    using FStorage = Rem::TFeatureRegistry<FTagEventSourceFamily>;

    using FContribution = FStorage::FContribution;
    using FRequest      = FStorage::FRequest;
    using FHandle       = FStorage::FHandle;

    template <typename TLiveness>
    using TTypedHandle = FStorage::TTypedHandle<TLiveness>;
};

/** Registers a contribution for as long as the object lives; `Reset()` clears it early. */
using FTagEventSourceRegistration = Rem::TFeatureRegistration<FTagEventSourceFamily>;

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
    FTagEventSourceFeature::FRequest Actor);

/** The family's typed derivation, as the handle traits define it. */
template <typename TLiveness>
[[nodiscard]] FTagEventSourceFeature::TTypedHandle<TLiveness> DeriveTypedHandle(
    const FTagEventSourceFeature::FHandle& Erased)
{
    return Rem::TFeatureHandleTraits<FTagEventSourceFamily>::template DeriveTypedHandle<TLiveness>(Erased);
}
} // namespace Private

/**
 * @brief The tag event source for @p Actor, bound to a concrete owner (liveness) type.
 * @note Asks the actor's getter first, then the contributions registered for @p TLiveness or for the
 * `UObject` wildcard.
 */
template <typename TLiveness>
[[nodiscard]] FTagEventSourceFeature::TTypedHandle<TLiveness> TryGetTagEventSource(
    const FTagEventSourceFeature::FRequest Actor)
{
    if (const FTagEventSourceFeature::FHandle FromActor = Private::TryGetActorProvidedSource(Actor);
        FromActor.IsValid())
    {
        return Private::DeriveTypedHandle<TLiveness>(FromActor);
    }

    return FTagEventSourceFamily::Instance.ProvideTyped<TLiveness>(Actor);
}
} // namespace Rem::GameplayTag
