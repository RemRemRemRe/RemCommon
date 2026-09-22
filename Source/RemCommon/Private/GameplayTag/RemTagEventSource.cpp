// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "GameplayTag/RemTagEventSourceFeature.h"

#include "GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h"

#include "GameFramework/Actor.h"
#include "Interface/RemGetLifetimeBoundObjectPtrInterface.h"
#include "Interface/RemGetLifetimeBoundOwnerInterface.h"
#include "Interface/RemGetTagEventSourceInterface.h"

namespace Rem::GameplayTag
{
/** the family's single registry: this definition is its one home */
Rem::TFeatureRegistry<FTagEventSourceFamily> FTagEventSourceFamily::Instance;

// the explicit instantiation keeps the walk's code in this module; the exported class makes the
// callers import it (Rem::TFeatureRegistry documents the reasoning)
template class Rem::TFeatureRegistry<FTagEventSourceFamily>;
template class Rem::TFeatureRegistration<FTagEventSourceFamily>;

namespace Private
{
FTagEventSourceFeature::FHandle TryGetActorProvidedSource(const FTagEventSourceFeature::FRequest& Actor)
{
    /// the not-null contract is checked at the boundary; the body works with the raw pointer
    AActor* ActorPtr = Actor;

    // the worker declares its own identity when it implements IRemGetLifetimeBoundOwnerInterface (a component
    // belongs to its actor, a self-implementing actor to itself); a worker without the protocol is
    // owned by the actor it was requested for
    const auto MakeHandle = [&](const TNotNull<IRemTagEventSourceInterface*> Worker)
    {
        if (const auto* Protocol = Cast<IRemGetLifetimeBoundOwnerInterface>(Cast<UObject>(Worker)))
        {
            return FTagEventSourceFeature::FHandle{Protocol->GetLifetimeBoundOwner(), Worker};
        }

        return FTagEventSourceFeature::FHandle{ActorPtr, Worker};
    };

    // 1. actor-provided worker
    if (auto* Getter = Cast<IRemGetTagEventSourceInterface>(ActorPtr))
    {
        if (auto* Worker = Getter->GetTagEventSource())
        {
            return MakeHandle(Worker);
        }
    }

    return {};
}
} // namespace Private

FTagEventSourceFeature::FHandle TryGetLifetimeBoundInterfacePtr(const TNotNull<AActor*> Actor)
{
    if (const FTagEventSourceFeature::FHandle FromActor = Private::TryGetActorProvidedSource(Actor);
        FromActor.IsValid())
    {
        return FromActor;
    }

    // the contribution chain: the first module that has a source wins
    return FTagEventSourceFamily::Instance.Provide(Actor);
}
} // namespace Rem::GameplayTag
