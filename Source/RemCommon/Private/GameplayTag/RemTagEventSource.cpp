// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "GameplayTag/RemTagEventSourceFeature.h"

#include "GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h"

#include "GameFramework/Actor.h"
#include "Interface/RemGetLifetimeBoundObjectPtrInterface.h"
#include "Interface/RemGetLifetimeBoundOwnerInterface.h"
#include "Interface/RemGetTagEventSourceInterface.h"
#include "Templates/Casts.h"

namespace Rem::GameplayTag
{
/** the family's single registry: this definition is its one home */
TFeatureRegistry<FTagEventSourceFamily> FTagEventSourceFamily::Instance;

// The family's instantiations are emitted and exported here, so a consumer can declare them `extern template`
// and link against this module. The two lines keep the `Rem::` qualifier: at a cross-namespace instantiation
// site, naming the owning namespace is the clearer spelling.
template class REMCOMMON_API Rem::TFeatureRegistry<FTagEventSourceFamily>;
template class REMCOMMON_API Rem::TFeatureRegistration<FTagEventSourceFamily>;

namespace Private
{
FTagEventSourceFeature::FHandle TryGetActorProvidedSource(const FTagEventSourceFeature::FRequest& Actor)
{
    // the not-null contract is checked at the boundary; the body works with the raw pointer
    AActor* const ActorPtr = Actor;

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

    if (auto* Getter = Cast<IRemGetTagEventSourceInterface>(ActorPtr))
    {
        if (auto* Worker = Getter->GetTagEventSource())
        {
            return MakeHandle(Worker);
        }
    }

    return {};
}
}

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
}
