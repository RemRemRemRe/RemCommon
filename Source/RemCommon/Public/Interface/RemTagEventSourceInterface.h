// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Containers/ArrayView.h"
#include "UObject/Interface.h"

#include "RemTagEventSourceInterface.generated.h"

struct FGameplayTagContainer;
struct FGameplayTag;

/**
 * generic gameplay tag event source
 *
 * the view target provides one either by implementing IRemGetTagEventSourceInterface
 * (returning its own worker), or through a contribution a module registers for the family
 * (e.g. an AbilitySystemComponent-backed component registered by a GAS-dependent module)
 *
 * a worker is a UObject and declares the object that bounds its lifetime through
 * IRemGetLifetimeBoundOwnerInterface; holders keep the family's handle
 * (GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h) so the source auto-invalidates when its owner dies
 *
 * the family and its typed query: GameplayTag/RemTagEventSourceFeature.h; how a module contributes one:
 * Templates/RemFeatureRegistry.h; the identity protocol: Interface/RemGetLifetimeBoundOwnerInterface.h
 */
UINTERFACE(MinimalAPI)
class URemTagEventSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class REMCOMMON_API IRemTagEventSourceInterface
{
    GENERATED_BODY()

public:
    using FEventDelegate = TMulticastDelegate<void(FGameplayTag, int32)>::FDelegate;

    /** current owned gameplay tags */
    [[nodiscard]] virtual const FGameplayTagContainer& GetOwnedTags() const = 0;

    /** add owned tags (e.g. loose gameplay tags on an AbilitySystemComponent) */
    virtual void AddOwnedTags(const FGameplayTagContainer& Tags) = 0;

    /**
     * add owned tags given as a view; the default implementation builds a container and forwards
     * to the container overload, so a subclass only needs the container override; an implementer
     * that declares the container overload must also pull this overload into scope (a using
     * declaration), or it stays hidden when the type is used concretely
     */
    virtual void AddOwnedTags(TConstArrayView<FGameplayTag> Tags);

    /** remove owned tags (e.g. loose gameplay tags on an AbilitySystemComponent) */
    virtual void RemoveOwnedTags(const FGameplayTagContainer& Tags) = 0;

    /**
     * remove owned tags given as a view; the default implementation builds a container and forwards
     * to the container overload, so a subclass only needs the container override; an implementer
     * that declares the container overload must also pull this overload into scope (a using
     * declaration), or it stays hidden when the type is used concretely
     */
    virtual void RemoveOwnedTags(TConstArrayView<FGameplayTag> Tags);

    /**
     * register a tag event, fired with (Tag, Count) when the tag count changes;
     * multiple listeners per tag are supported
     *
     * the returned handle identifies the bound delegate instance, not one registration:
     * registering the same instance for several tags yields the same handle, so pass the tag
     * back on unregister to pick which binding to remove
     */
    virtual FDelegateHandle RegisterTagEvent(const FGameplayTag& Tag, const FEventDelegate& Delegate) = 0;

    /**
     * unregister one registration: the tag selects the binding when a single delegate
     * instance was registered for several tags
     */
    virtual void UnregisterTagEvent(const FGameplayTag& Tag, FDelegateHandle Handle) = 0;

    /** unregister every tag event this source remembers */
    virtual void UnregisterAllTagEvents() = 0;
};
