// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"

#include "Interface/RemTagEventSourceInterface.h"
#include "Templates/RemLifetimeBoundPtr.h"
#include "RemNotNull.h"

class AActor;

namespace Rem::GameplayTag
{
/**
 * reference to a tag event source: the object that bounds its lifetime (usually the view target
 * actor the source belongs to) plus the source itself; weak, and self-invalidating
 *
 * kept apart from the interface header so that including the interface stays a pure interface
 * include; a consumer that only holds such a reference needs this file, a consumer that registers a
 * contribution needs GameplayTag/RemTagEventSourceFeature.h
 */
using FLifetimeBoundInterfacePtr = TLifetimeBoundPtr<TWeakObjectPtr<UObject>, IRemTagEventSourceInterface>;

/**
 * create a tag event source for an actor:
 * 1. if the actor implements IRemGetTagEventSourceInterface, use its returned source
 * 2. otherwise, ask the contributions a module registered for the family (in registration order)
 *
 * the handle's liveness is the actor this was requested for, or the owner the worker declares
 * through IRemGetLifetimeBoundOwnerInterface (a worker that belongs to another object, e.g. a component
 * whose owner is the view target)
 *
 * @see GameplayTag/RemTagEventSourceFeature.h - the typed query and the family's public surface;
 * Templates/RemFeatureRegistry.h - the mechanism a module registers a contribution through
 * @return an invalid reference when neither path yields a source
 */
REMCOMMON_API [[nodiscard]] FLifetimeBoundInterfacePtr TryGetLifetimeBoundInterfacePtr(TNotNull<AActor*> Actor);
}
