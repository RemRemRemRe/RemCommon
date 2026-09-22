// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/RemTagEventSourceInterface.h"
#include "Interface/RemGetTagEventSourceInterface.h"
#include "Interface/RemGetLifetimeBoundOwnerInterface.h"
#include "Interface/RemGetLifetimeBoundObjectPtrInterface.h"
#include "UObject/NoExportTypes.h"

// generated.h MUST be included BEFORE the type declarations: GENERATED_BODY()
// expands to a macro defined here; a trailing include leaves it undefined (C4430).
// UHT emits a matching forward declaration inside the namespace, so the include
// stays outside it.
#include "RemTagEventSourceTestTypes.generated.h"

namespace Rem::Common::Private
{
/**
 * minimal in-repo worker implementing the tag event source interface
 */
UCLASS()
class URemTestTagEventWorker : public UObject, public IRemTagEventSourceInterface,
                               public IRemGetLifetimeBoundOwnerInterface,
                               public TGetLifetimeBoundObjectPtr<URemTestTagEventWorker>
{
    GENERATED_BODY()

public:
    // IRemGetLifetimeBoundOwnerInterface
    // a bare worker has no owner actor: its identity is itself
    [[nodiscard]] virtual UObject* GetLifetimeBoundOwner() const override;

    FGameplayTagContainer OwnedTags;
    TMap<FGameplayTag, TMulticastDelegate<void(FGameplayTag, int32)>> Events;
    TMap<FDelegateHandle, FGameplayTag> HandleToTag;

    // IRemTagEventSourceInterface
    virtual const FGameplayTagContainer& GetOwnedTags() const override;
    virtual void AddOwnedTags(const FGameplayTagContainer& TagsToAdd) override;
    virtual void RemoveOwnedTags(const FGameplayTagContainer& TagsToRemove) override;
    virtual FDelegateHandle RegisterTagEvent(const FGameplayTag& Tag, const FEventDelegate& Delegate) override;
    virtual void UnregisterTagEvent(FDelegateHandle Handle) override;
    virtual void UnregisterAllTagEvents() override;
};

/**
 * USTRUCT implementer: the typed accessor resolves the interface with a compile-time cast, which is
 * the only path that can work here - UHT generates _getUObject() for classes only, so
 * Cast<UObject> of a struct's interface yields null
 */
USTRUCT()
struct FRemTestLifetimeBoundStruct
#if CPP
    : public IRemGetLifetimeBoundOwnerInterface
#endif
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UObject> Owner{};

    // the owner is the object this struct names
    [[nodiscard]] virtual UObject* GetLifetimeBoundOwner() const override;
};

/**
 * actor implementing the getter interface, providing its own worker
 */
UCLASS()
class ARemTestTagEventGetterActor : public AActor, public IRemGetTagEventSourceInterface
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TObjectPtr<URemTestTagEventWorker> Worker{};

    ARemTestTagEventGetterActor();

    // IRemGetTagEventSourceInterface
    virtual IRemTagEventSourceInterface* GetTagEventSource() override;
    virtual const IRemTagEventSourceInterface* GetTagEventSource() const override;
};

/**
 * actor implementing the getter interface but returning null — the query must
 * fall through to the registered contribution
 */
UCLASS()
class ARemTestNullGetterActor : public AActor, public IRemGetTagEventSourceInterface
{
    GENERATED_BODY()

public:
    ARemTestNullGetterActor();

    // IRemGetTagEventSourceInterface
    virtual IRemTagEventSourceInterface* GetTagEventSource() override;
    virtual const IRemTagEventSourceInterface* GetTagEventSource() const override;
};

// --- inline definitions at file bottom ---

inline UObject* URemTestTagEventWorker::GetLifetimeBoundOwner() const
{
    return const_cast<URemTestTagEventWorker*>(this);
}

inline const FGameplayTagContainer& URemTestTagEventWorker::GetOwnedTags() const
{
    return OwnedTags;
}

inline void URemTestTagEventWorker::AddOwnedTags(const FGameplayTagContainer& TagsToAdd)
{
    OwnedTags.AppendTags(TagsToAdd);
}

inline void URemTestTagEventWorker::RemoveOwnedTags(const FGameplayTagContainer& TagsToRemove)
{
    OwnedTags.RemoveTags(TagsToRemove);
}

inline FDelegateHandle URemTestTagEventWorker::RegisterTagEvent(const FGameplayTag& Tag,
    const FEventDelegate& Delegate)
{
    auto& Multicast   = Events.FindOrAdd(Tag);
    const auto Handle = Multicast.Add(Delegate);
    HandleToTag.Add(Handle, Tag);
    return Handle;
}

inline void URemTestTagEventWorker::UnregisterTagEvent(const FDelegateHandle Handle)
{
    if (const auto* Tag = HandleToTag.Find(Handle))
    {
        if (auto* Multicast = Events.Find(*Tag))
        {
            Multicast->Remove(Handle);
        }
        HandleToTag.Remove(Handle);
    }
}

inline void URemTestTagEventWorker::UnregisterAllTagEvents()
{
    Events.Reset();
    HandleToTag.Reset();
}

inline UObject* FRemTestLifetimeBoundStruct::GetLifetimeBoundOwner() const
{
    return Owner;
}

inline ARemTestTagEventGetterActor::ARemTestTagEventGetterActor()
{
    // bare actors have no root component; give the actor a scene root so it
    // can be placed in the test world reliably
    auto* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent   = SceneRoot;
}

inline IRemTagEventSourceInterface* ARemTestTagEventGetterActor::GetTagEventSource()
{
    return Worker;
}

inline const IRemTagEventSourceInterface* ARemTestTagEventGetterActor::GetTagEventSource() const
{
    return Worker;
}

inline ARemTestNullGetterActor::ARemTestNullGetterActor()
{
    auto* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent   = SceneRoot;
}

inline IRemTagEventSourceInterface* ARemTestNullGetterActor::GetTagEventSource()
{
    return nullptr;
}

inline const IRemTagEventSourceInterface* ARemTestNullGetterActor::GetTagEventSource() const
{
    return nullptr;
}
}
