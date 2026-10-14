// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

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

    /** remove owned tags (e.g. loose gameplay tags on an AbilitySystemComponent) */
    virtual void RemoveOwnedTags(const FGameplayTagContainer& Tags) = 0;

    /**
     * register a tag event, fired with (Tag, Count) when the tag count changes;
     * multiple listeners per tag are supported
     *
     * the returned handle identifies the bound delegate instance, not one registration:
     * pass a distinct instance per registration, because registering the same instance
     * for several tags yields the same handle
     */
    virtual FDelegateHandle RegisterTagEvent(const FGameplayTag& Tag, const FEventDelegate& Delegate) = 0;

    /**
     * unregister one registration by its handle; when several registrations share a
     * delegate instance, one call removes only one of them
     */
    virtual void UnregisterTagEvent(FDelegateHandle Handle) = 0;

    /**
     * unregister every tag event this source remembers; when a delegate instance was registered
     * for several tags, its handle maps to only one of them, so the other bindings are not
     * reachable through this interface
     */
    virtual void UnregisterAllTagEvents() = 0;
};
