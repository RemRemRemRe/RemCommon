// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemTestWorld.h"

#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DataTable.h"
#include "Misc/AutomationTest.h"
#include "Object/RemObjectStatics.inl"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Object::GetGameInstance;
using Rem::Object::GetGameState;
using Rem::Object::GetPlayerState;
using Rem::Object::FindMovementComponent;
using Rem::Object::SpawnActor;
using Rem::Object::GetHashForObjects;
using Rem::Object::ObjectPtrDecay;
using Rem::Object::MakeArrayView;
}

DEFINE_SPEC(FRemObjectStaticsTest, "Rem.Common.Object",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemObjectStaticsTest::Define()
{
    Describe(TEXT("World accessors without a player"), [this]
    {
        It(TEXT("GetGameInstance should return null when the world has no game instance"), [this]
        {
            Rem::Test::FRemTestWorld World;

            const auto* Result = GetGameInstance(World);

            TestTrue(TEXT("no game instance"), Result == nullptr);
        });

        It(TEXT("GetGameState should return null when the world has no game state"), [this]
        {
            Rem::Test::FRemTestWorld World;

            TestTrue(TEXT("no game state"), GetGameState(World) == nullptr);
        });

        It(TEXT("GetPlayerState should return null for an actor without a player state"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();

            // TNotNull<TActorOfNotNull*> is a non-deduced context: the wrapper must be explicit
            const Rem::TNotNull<AActor*> NotNullActor{Actor};
            const auto* Result = GetPlayerState(NotNullActor);

            TestTrue(TEXT("no player state"), Result == nullptr);
        });
    });

    Describe(TEXT("SpawnActor"), [this]
    {
        It(TEXT("should spawn and finish an actor in the world"), [this]
        {
            Rem::Test::FRemTestWorld World;

            auto* Actor = SpawnActor<AActor>(World, AActor::StaticClass(), FTransform::Identity);

            TestTrue(TEXT("spawned"), Actor != nullptr);
            TestTrue(TEXT("in world"), Actor->GetWorld() == World);
        });

        It(TEXT("should spawn an actor with an owner"), [this]
        {
            Rem::Test::FRemTestWorld World;

            auto* OwnerActor = World->SpawnActor<AActor>();
            auto* Actor = SpawnActor<AActor>(World, AActor::StaticClass(), FTransform::Identity,
                ESpawnActorCollisionHandlingMethod::Undefined, OwnerActor);

            TestTrue(TEXT("spawned"), Actor != nullptr);
            TestTrue(TEXT("owned"), Actor->GetOwner() == OwnerActor);
        });

        It(TEXT("should spawn relative to an owner actor"), [this]
        {
            Rem::Test::FRemTestWorld World;

            auto* OwnerActor = World->SpawnActor<AActor>();
            auto* Actor = SpawnActor<AActor>(OwnerActor, AActor::StaticClass(), FTransform::Identity);

            TestTrue(TEXT("spawned"), Actor != nullptr);
            TestTrue(TEXT("owned"), Actor->GetOwner() == OwnerActor);
        });
    });

    Describe(TEXT("FindComponentByClass"), [this]
    {
        It(TEXT("should find a movement component on a character"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Character = World->SpawnActor<ACharacter>();

            const Rem::TNotNull<ACharacter*> NotNullCharacter{Character};
            const auto* Found = FindMovementComponent<UCharacterMovementComponent>(NotNullCharacter);

            TestTrue(TEXT("movement component found"), Found != nullptr);
            TestTrue(TEXT("is the character movement"), Found->GetOwner() == Character);
        });

        It(TEXT("should return null when the component type is missing"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Character = World->SpawnActor<ACharacter>();

            const Rem::TNotNull<ACharacter*> NotNullCharacter{Character};
            const auto* Found = FindMovementComponent<UFloatingPawnMovement>(NotNullCharacter);

            TestTrue(TEXT("not found"), Found == nullptr);
        });
    });

    Describe(TEXT("Object collections"), [this]
    {
        It(TEXT("GetHashForObjects should be stable for the same objects"), [this]
        {
            auto* ObjectA = NewObject<UDataTable>();
            auto* ObjectB = NewObject<UDataTable>();

            const TArray<UObject*> ObjectsOne{ObjectA, ObjectB};
            const TArray<UObject*> ObjectsTwo{ObjectA, ObjectB};

            TestEqual(TEXT("stable hash"), GetHashForObjects<UObject>(ObjectsOne),
                GetHashForObjects<UObject>(ObjectsTwo));
        });

        It(TEXT("ObjectPtrDecay should decay a TObjectPtr array in place"), [this]
        {
            auto* ObjectA = NewObject<UDataTable>();
            auto* ObjectB = NewObject<UDataTable>();
            TArray<TObjectPtr<UObject>> ObjectPtrs{ObjectA, ObjectB};

            auto& Decayed = ObjectPtrDecay(ObjectPtrs);

            TestEqual(TEXT("decayed count"), Decayed.Num(), 2);
            TestTrue(TEXT("first object"), Decayed[0] == ObjectA);
            TestTrue(TEXT("second object"), Decayed[1] == ObjectB);
        });

        It(TEXT("MakeArrayView should view a TObjectPtr array as raw pointers"), [this]
        {
            auto* ObjectA = NewObject<UDataTable>();
            TArray<TObjectPtr<UObject>> ObjectPtrs{ObjectA};

            const auto View = MakeArrayView(ObjectPtrs);

            TestEqual(TEXT("view count"), View.Num(), 1);
            TestTrue(TEXT("viewed object"), View[0] == ObjectA);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
