// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemTagEventSourceTestTypes.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interface/RemTagEventSourceInterface.h"
#include "GameplayTag/RemTagEventLifetimeBoundInterfacePtr.h"
#include "GameplayTag/RemTagEventSourceFeature.h"
#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Common::Private::ARemTestNullGetterActor;
using Rem::Common::Private::ARemTestTagEventGetterActor;
using Rem::Common::Private::FRemTestLifetimeBoundStruct;
using Rem::Common::Private::URemTestTagEventWorker;

/** a contribution that applies to every actor and hands out @p Worker */
Rem::GameplayTag::FTagEventSourceFeature::FContribution MakeWorkerContribution(URemTestTagEventWorker* Worker)
{
    return {
        UObject::StaticClass(),
        [Worker](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
        {
            return Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
        }
    };
}

/** worker owned by the given actor, so it stays alive for the test scope */
URemTestTagEventWorker* MakeWorker(AActor* Owner)
{
    return NewObject<URemTestTagEventWorker>(Owner);
}
} // namespace

DEFINE_SPEC(FRemTagEventSourceTest, "Rem.Common.GameplayTagEventSource",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemTagEventSourceTest::Define()
{
    Describe(TEXT("TryGetLifetimeBoundInterfacePtr"), [this]
    {
        It(TEXT("should return the actor-provided worker when the actor implements the getter"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor   = World->SpawnActor<ARemTestTagEventGetterActor>();
            Actor->Worker = NewObject<URemTestTagEventWorker>(Actor);

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("source is the actor's worker"), Source.GetObject() == Actor->Worker);
        });

        It(TEXT("should return the provided source for an actor without a getter"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor          = World->SpawnActor<ARemTestNullGetterActor>();
            auto* ProvidedWorker = MakeWorker(Actor);

            // the contribution lives for this scope and is cleared when it ends
            const Rem::GameplayTag::FTagEventSourceRegistration Registration{MakeWorkerContribution(ProvidedWorker)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("source is the contribution's worker"), Source.GetObject() == ProvidedWorker);
        });

        It(TEXT("should prefer the actor getter over a contribution"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor          = World->SpawnActor<ARemTestTagEventGetterActor>();
            Actor->Worker        = NewObject<URemTestTagEventWorker>(Actor);
            auto* ProvidedWorker = MakeWorker(Actor);

            const Rem::GameplayTag::FTagEventSourceRegistration Registration{MakeWorkerContribution(ProvidedWorker)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("actor worker wins over the contribution"), Source.GetObject() == Actor->Worker);
        });

        It(TEXT("should return the provided source for a plain actor"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor          = World->SpawnActor<AActor>();
            auto* ProvidedWorker = MakeWorker(Actor);

            const Rem::GameplayTag::FTagEventSourceRegistration Registration{MakeWorkerContribution(ProvidedWorker)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("source is the contribution's worker"), Source.GetObject() == ProvidedWorker);
        });

        It(TEXT("should return an invalid source when the contribution yields none"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();

            // having nothing to give is a valid answer: the chain simply has no source
            const Rem::GameplayTag::FTagEventSourceRegistration Registration{
                {
                    UObject::StaticClass(),
                    [](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    -> Rem::GameplayTag::FLifetimeBoundInterfacePtr
                    {
                        return {};
                    },
                }
            };

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("invalid"), !Source.IsValid());
        });

        It(TEXT("should return an invalid source with no getter and no contribution"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("invalid"), !Source.IsValid());
        });
    });

    Describe(TEXT("Contribution chain"), [this]
    {
        It(TEXT("should keep walking when a contribution yields none"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            const Rem::GameplayTag::FTagEventSourceRegistration Empty{
                {
                    UObject::StaticClass(),
                    [](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    -> Rem::GameplayTag::FLifetimeBoundInterfacePtr
                    {
                        return {};
                    },
                }
            };
            const Rem::GameplayTag::FTagEventSourceRegistration Providing{MakeWorkerContribution(Worker)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("the later contribution answers"), Source.IsValid());
            TestTrue(TEXT("it is the later contribution's worker"), Source.GetObject() == Worker);
        });

        It(TEXT("should keep the first contribution when two claim the same owner type"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* First  = MakeWorker(Actor);
            auto* Second = MakeWorker(Actor);

            // both claim the same key: registration order decides, which is the contract
            const Rem::GameplayTag::FTagEventSourceRegistration FirstRegistered{MakeWorkerContribution(First)};
            const Rem::GameplayTag::FTagEventSourceRegistration SecondRegistered{MakeWorkerContribution(Second)};

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("the first registered contribution wins"), Source.GetObject() == First);
        });

        It(TEXT("should keep the registration order of the remaining contributions after a removal"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* First  = MakeWorker(Actor);
            auto* Second = MakeWorker(Actor);
            auto* Third  = MakeWorker(Actor);

            // removing the first must leave the survivors in registration order, not swap the
            // last-registered one into the freed slot
            Rem::GameplayTag::FTagEventSourceRegistration FirstRegistered{MakeWorkerContribution(First)};
            const Rem::GameplayTag::FTagEventSourceRegistration SecondRegistered{MakeWorkerContribution(Second)};
            const Rem::GameplayTag::FTagEventSourceRegistration ThirdRegistered{MakeWorkerContribution(Third)};

            FirstRegistered.Reset();

            const auto Source = Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor);

            TestTrue(TEXT("valid source"), Source.IsValid());
            TestTrue(TEXT("the next registered contribution answers, not the last one"),
                Source.GetObject() == Second);
        });

        It(TEXT("should not let a later wildcard contribution overtake an earlier typed one when a contribution is removed"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor          = World->SpawnActor<AActor>();
            auto* TypedWorker    = MakeWorker(Actor);
            auto* WildcardWorker = MakeWorker(Actor);

            // the typed query is what makes the owner keys decide: entry 1 is typed but yields
            // nothing, and removing it must not move the trailing wildcard ahead of the earlier
            // typed contribution
            Rem::GameplayTag::FTagEventSourceRegistration Empty{
                {
                    URemTestTagEventWorker::StaticClass(),
                    [](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    -> Rem::GameplayTag::FLifetimeBoundInterfacePtr
                    {
                        return {};
                    },
                }
            };
            const Rem::GameplayTag::FTagEventSourceRegistration Typed{
                {
                    URemTestTagEventWorker::StaticClass(),
                    [TypedWorker](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    {
                        return TypedWorker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
                    },
                }
            };
            const Rem::GameplayTag::FTagEventSourceRegistration Wildcard{MakeWorkerContribution(WildcardWorker)};

            Empty.Reset();

            const auto TypedSource = Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor);

            TestTrue(TEXT("valid typed source"), TypedSource.IsValid());
            TestTrue(TEXT("the earlier typed contribution answers, not the wildcard"),
                TypedSource.GetObject() == TypedWorker);
        });

        It(TEXT("should forget a contribution once it is reset"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            Rem::GameplayTag::FTagEventSourceRegistration Registration{MakeWorkerContribution(Worker)};
            TestTrue(TEXT("registered"), Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).IsValid());

            Registration.Reset();
            TestTrue(TEXT("cleared"), !Rem::GameplayTag::TryGetLifetimeBoundInterfacePtr(Actor).IsValid());
        });
    });

    Describe(TEXT("Typed query"), [this]
    {
        It(TEXT("should take the contribution registered for the queried owner type"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            // registered for the actor type, so the typed query for the worker type must skip it
            const Rem::GameplayTag::FTagEventSourceRegistration ForActorType{
                {
                    AActor::StaticClass(),
                    [Worker](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    {
                        return Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
                    },
                }
            };
            TestTrue(TEXT("a query for another owner type finds nothing"),
                !Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor).IsValid());

            const Rem::GameplayTag::FTagEventSourceRegistration ForWorkerType{
                {
                    URemTestTagEventWorker::StaticClass(),
                    [Worker](const Rem::GameplayTag::FTagEventSourceFeature::FRequest)
                    {
                        return Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
                    },
                }
            };

            const auto TypedSource = Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor);

            static_assert(decltype(TypedSource)::bCompactStorage,
                "a self-implementing worker should reach the compact storage");
            TestTrue(TEXT("typed source is valid"), TypedSource.IsValid());
            TestTrue(TEXT("typed identity is the worker"), TypedSource.GetObject() == Worker);
            TestTrue(TEXT("typed source is the worker"),
                TypedSource.TryGet() == static_cast<IRemTagEventSourceInterface*>(Worker));
        });

        It(TEXT("should let a wildcard contribution answer any typed query"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Actor  = World->SpawnActor<AActor>();
            auto* Worker = MakeWorker(Actor);

            // the wildcard is registered for UObject, so it is visible to every typed query
            const Rem::GameplayTag::FTagEventSourceRegistration Wildcard{MakeWorkerContribution(Worker)};

            const auto TypedSource = Rem::GameplayTag::TryGetTagEventSource<URemTestTagEventWorker>(Actor);

            TestTrue(TEXT("typed source is valid"), TypedSource.IsValid());
            TestTrue(TEXT("typed identity is the worker"), TypedSource.GetObject() == Worker);
        });
    });

    Describe(TEXT("IRemGetLifetimeBoundOwnerInterface"), [this]
    {
        It(TEXT("should give a self-implementing worker the compact handle"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Owner  = World->SpawnActor<AActor>();
            auto* Worker = NewObject<URemTestTagEventWorker>(Owner);

            // the worker reports itself as its owner, so the pointee (the interface it implements)
            // is derivable from the liveness payload at compile time
            const auto Handle = Worker->GetLifetimeBoundObjectPtr<IRemTagEventSourceInterface>();
            static_assert(decltype(Handle)::bCompactStorage,
                "a self-implementing worker should reach the compact storage");
            TestEqual(TEXT("compact handle is one weak pointer wide"),
                static_cast<int32>(sizeof(Handle)), static_cast<int32>(sizeof(TWeakObjectPtr<URemTestTagEventWorker>)));
            TestTrue(TEXT("identity is the worker itself"), Handle.GetObject() == Worker);
            TestTrue(TEXT("the pointee is the worker"),
                Handle.TryGet() == static_cast<IRemTagEventSourceInterface*>(Worker));

            // and it still converts to the erased handle a consumer stores
            const Rem::GameplayTag::FLifetimeBoundInterfacePtr Erased{Handle};
            TestTrue(TEXT("the erasure bridge keeps the worker"), Erased.TryGet() == Handle.TryGet());
        });

        It(TEXT("should keep the general storage for a worker owned by another object"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Owner = World->SpawnActor<AActor>();

            FRemTestLifetimeBoundStruct Struct{};
            Struct.Owner = Owner;

            // the USTRUCT accessor takes the erased default liveness, so the handle is general
            const auto Handle = Rem::GetLifetimeBoundObjectPtr<IRemGetLifetimeBoundOwnerInterface>(Struct);
            static_assert(!decltype(Handle)::bCompactStorage,
                "an erased liveness cannot derive the pointee");
            TestEqual(TEXT("general handle is a weak pointer plus the pointee"),
                static_cast<int32>(sizeof(Handle)),
                static_cast<int32>(sizeof(TWeakObjectPtr<UObject>) + sizeof(void*)));
        });

        It(TEXT("should resolve the interface of a USTRUCT implementer through the typed accessor"), [this]
        {
            const Rem::Test::FRemTestWorld World;
            auto* Owner = World->SpawnActor<AActor>();

            FRemTestLifetimeBoundStruct Struct{};
            Struct.Owner = Owner;

            const auto Handle = Rem::GetLifetimeBoundObjectPtr<IRemGetLifetimeBoundOwnerInterface>(Struct);

            TestTrue(TEXT("identity is the owner"), Handle.GetObject() == Owner);
            TestTrue(TEXT("the handle points at the struct"), Handle.TryGet() == &Struct);

            // the accessor is the only path for a struct: no UObject is behind its interface
            TestNull(TEXT("no UObject behind a struct's interface"),
                Cast<UObject>(static_cast<IRemGetLifetimeBoundOwnerInterface*>(&Struct)));
        });

    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
