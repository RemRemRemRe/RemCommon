// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/RemTagEventSourceInterface.h"
#include "Interface/RemGetTagEventSourceInterface.h"
#include "Interface/RemGetLifetimeBoundOwnerInterface.h"
#include "Interface/RemGetLifetimeBoundObjectPtrInterface.h"
#include "Templates/Casts.h"
#include "Templates/RemFeatureRegistry.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakObjectPtrTemplates.h"

// generated.h MUST be included BEFORE the type declarations: GENERATED_BODY()
// expands to a macro defined here; a trailing include leaves it undefined (C4430).
// UHT emits a matching forward declaration inside the namespace, so the include
// stays outside it.
#include "RemTagEventSourceTestTypes.generated.h"

namespace Rem::Common::Private
{
/**
 * minimal in-repo worker implementing only the tag event source interface: it declares no identity,
 * so a handle built from it takes the identity of the object the query named
 */
UCLASS()
class URemTestBareTagEventWorker : public UObject, public IRemTagEventSourceInterface
{
    GENERATED_BODY()

public:
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
 * minimal in-repo worker implementing the tag event source interface plus the identity protocol; a
 * self-identifying worker is its own owner, so a handle built from it is compact
 */
UCLASS()
class URemTestTagEventWorker : public URemTestBareTagEventWorker,
                               public IRemGetLifetimeBoundOwnerInterface,
                               public TGetLifetimeBoundObjectPtr<URemTestTagEventWorker>
{
    GENERATED_BODY()

public:
    // IRemGetLifetimeBoundOwnerInterface
    // a self-identifying worker has no owner actor: its identity is itself
    [[nodiscard]] virtual UObject* GetLifetimeBoundOwner() const override;
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

/**
 * actor whose getter hands out a worker that implements only the tag event source interface, so the
 * query has no identity protocol to consult
 */
UCLASS()
class ARemTestBareWorkerGetterActor : public AActor, public IRemGetTagEventSourceInterface
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TObjectPtr<URemTestBareTagEventWorker> Worker{};

    ARemTestBareWorkerGetterActor();

    // IRemGetTagEventSourceInterface
    virtual IRemTagEventSourceInterface* GetTagEventSource() override;
    virtual const IRemTagEventSourceInterface* GetTagEventSource() const override;
};

/**
 * tag event source component: its identity is the actor that owns it, so the handle it hands out
 * takes the general storage - the pointee (the component) is not derivable from the liveness payload
 */
UCLASS()
class URemTestTagEventSourceComponent : public USceneComponent, public IRemTagEventSourceInterface,
                                        public IRemGetLifetimeBoundOwnerInterface,
                                        public TGetLifetimeBoundObjectPtr<URemTestTagEventSourceComponent, AActor>
{
    GENERATED_BODY()

public:
    /** the component is the source; its worker holds the tag event bookkeeping */
    UPROPERTY()
    TObjectPtr<URemTestBareTagEventWorker> Worker{};

    URemTestTagEventSourceComponent();

    // IRemGetLifetimeBoundOwnerInterface
    [[nodiscard]] virtual UObject* GetLifetimeBoundOwner() const override;

    // IRemTagEventSourceInterface
    virtual const FGameplayTagContainer& GetOwnedTags() const override;
    virtual void AddOwnedTags(const FGameplayTagContainer& TagsToAdd) override;
    virtual void RemoveOwnedTags(const FGameplayTagContainer& TagsToRemove) override;
    virtual FDelegateHandle RegisterTagEvent(const FGameplayTag& Tag, const FEventDelegate& Delegate) override;
    virtual void UnregisterTagEvent(FDelegateHandle Handle) override;
    virtual void UnregisterAllTagEvents() override;
};

/**
 * a family whose traits specialization supplies its own handle shape: the registry's documented extension
 * point, and the case of a module owning a family - the test module declares it, defines its registry and
 * emits its instantiations in `RemFeatureRegistryTestFamily.cpp`
 */
struct FRemTestCustomHandleFamily
{
    using FRequest = TNotNull<AActor*>;
    using FPointee = IRemTagEventSourceInterface;

    /** The family's registry: declared here, defined once in the family's `.cpp`. */
    REMCOMMONTEST_API static TFeatureRegistry<FRemTestCustomHandleFamily> Instance;
};

/** the custom family's erased handle: the mechanism asks it only for `IsValid()` */
struct FRemTestCustomHandle
{
    TWeakObjectPtr<UObject> Liveness{};
    IRemTagEventSourceInterface* Worker{};

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] IRemTagEventSourceInterface* TryGet() const;
};

/** the custom family's typed handle: the same worker, carrying the liveness type the caller named */
template <typename TLiveness>
struct TRemTestCustomTypedHandle
{
    TWeakObjectPtr<TLiveness> Liveness{};
    IRemTagEventSourceInterface* Worker{};

    [[nodiscard]] bool IsValid() const;

    [[nodiscard]] TLiveness* GetObject() const;

    [[nodiscard]] IRemTagEventSourceInterface* TryGet() const;
};

// --- inline definitions at file bottom ---

inline const FGameplayTagContainer& URemTestBareTagEventWorker::GetOwnedTags() const
{
    return OwnedTags;
}

inline void URemTestBareTagEventWorker::AddOwnedTags(const FGameplayTagContainer& TagsToAdd)
{
    OwnedTags.AppendTags(TagsToAdd);
}

inline void URemTestBareTagEventWorker::RemoveOwnedTags(const FGameplayTagContainer& TagsToRemove)
{
    OwnedTags.RemoveTags(TagsToRemove);
}

inline FDelegateHandle URemTestBareTagEventWorker::RegisterTagEvent(const FGameplayTag& Tag,
    const FEventDelegate& Delegate)
{
    auto& Multicast   = Events.FindOrAdd(Tag);
    const auto Handle = Multicast.Add(Delegate);
    HandleToTag.Add(Handle, Tag);
    return Handle;
}

inline void URemTestBareTagEventWorker::UnregisterTagEvent(const FDelegateHandle Handle)
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

inline void URemTestBareTagEventWorker::UnregisterAllTagEvents()
{
    Events.Reset();
    HandleToTag.Reset();
}

inline UObject* URemTestTagEventWorker::GetLifetimeBoundOwner() const
{
    return const_cast<URemTestTagEventWorker*>(this);
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

inline ARemTestBareWorkerGetterActor::ARemTestBareWorkerGetterActor()
{
    // bare actors have no root component; give the actor a scene root so it
    // can be placed in the test world reliably
    auto* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent   = SceneRoot;
}

inline IRemTagEventSourceInterface* ARemTestBareWorkerGetterActor::GetTagEventSource()
{
    return Worker;
}

inline const IRemTagEventSourceInterface* ARemTestBareWorkerGetterActor::GetTagEventSource() const
{
    return Worker;
}

inline URemTestTagEventSourceComponent::URemTestTagEventSourceComponent()
{
    Worker = CreateDefaultSubobject<URemTestBareTagEventWorker>(TEXT("Worker"));
}

inline UObject* URemTestTagEventSourceComponent::GetLifetimeBoundOwner() const
{
    return GetOwner();
}

inline const FGameplayTagContainer& URemTestTagEventSourceComponent::GetOwnedTags() const
{
    return Worker->GetOwnedTags();
}

inline void URemTestTagEventSourceComponent::AddOwnedTags(const FGameplayTagContainer& TagsToAdd)
{
    Worker->AddOwnedTags(TagsToAdd);
}

inline void URemTestTagEventSourceComponent::RemoveOwnedTags(const FGameplayTagContainer& TagsToRemove)
{
    Worker->RemoveOwnedTags(TagsToRemove);
}

inline FDelegateHandle URemTestTagEventSourceComponent::RegisterTagEvent(const FGameplayTag& Tag,
    const FEventDelegate& Delegate)
{
    return Worker->RegisterTagEvent(Tag, Delegate);
}

inline void URemTestTagEventSourceComponent::UnregisterTagEvent(const FDelegateHandle Handle)
{
    Worker->UnregisterTagEvent(Handle);
}

inline void URemTestTagEventSourceComponent::UnregisterAllTagEvents()
{
    Worker->UnregisterAllTagEvents();
}

inline bool FRemTestCustomHandle::IsValid() const
{
    return Liveness.IsValid() && Worker != nullptr;
}

inline IRemTagEventSourceInterface* FRemTestCustomHandle::TryGet() const
{
    return IsValid()
               ? Worker
               : nullptr;
}

template <typename TLiveness>
bool TRemTestCustomTypedHandle<TLiveness>::IsValid() const
{
    return Liveness.IsValid() && Worker != nullptr;
}

template <typename TLiveness>
TLiveness* TRemTestCustomTypedHandle<TLiveness>::GetObject() const
{
    return Liveness.Get();
}

template <typename TLiveness>
IRemTagEventSourceInterface* TRemTestCustomTypedHandle<TLiveness>::TryGet() const
{
    return IsValid()
               ? Worker
               : nullptr;
}
}

/**
 * the custom family's handle shape: the registry's documented extension point, exercised by the spec both
 * through the trait directly and through the walk of the family the test module owns
 */
template <>
struct Rem::TFeatureHandleTraits<Rem::Common::Private::FRemTestCustomHandleFamily>
{
    using FHandle = Common::Private::FRemTestCustomHandle;

    template <typename TLiveness>
    using TTypedHandle = Common::Private::TRemTestCustomTypedHandle<TLiveness>;

    /** the family's own derivation: the erased liveness, narrowed to the type the caller named */
    template <typename TLiveness>
    [[nodiscard]] static TTypedHandle<TLiveness> DeriveTypedHandle(const FHandle& Erased)
    {
        return TTypedHandle<TLiveness>{
            TWeakObjectPtr<TLiveness>{Cast<TLiveness>(Erased.Liveness.Get())},
            Erased.Worker
        };
    }
};

namespace Rem::Common::Private
{
// The test module owns this family, so it emits the family's instantiations; the undecorated declarations only say
// "do not instantiate locally" and the symbols come from the exporting module - the same shape every family has.
// They follow the traits specialization above on purpose: seeing an `extern template` instantiates the registry
// (MSVC), and the registry's handle type has to come from the specialization, not from the primary template.
extern template class Rem::TFeatureRegistry<FRemTestCustomHandleFamily>;
extern template class Rem::TFeatureRegistration<FRemTestCustomHandleFamily>;
}
