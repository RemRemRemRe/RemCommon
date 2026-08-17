// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Animation/RemAnimationStatics.h"
#include "Interface/RemAnimInstanceInterface.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Misc/AutomationTest.h"
#include "RemNotNull.h"
#include "RemTestWorld.h"

#include "Macro/RemPrivateMemberAccessor.h"

#if WITH_DEV_AUTOMATION_TESTS

REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(FCharacterMeshAccessor,
    &ACharacter::Mesh,
    TObjectPtr<USkeletalMeshComponent> ACharacter::*);

DEFINE_SPEC(FRemAnimInstanceInterfaceTest, "Rem.Common.AnimInstanceInterface",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemAnimInstanceInterfaceTest::Define()
{
    Describe(TEXT("GetAnimInstance"), [this]
    {
        It(TEXT("should return null for a character without an anim instance"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Character = World->SpawnActor<ACharacter>();
            check(Character);

            // a skeletal mesh without a configured anim blueprint has no anim instance
            auto* Mesh = NewObject<USkeletalMeshComponent>(Character);
            check(Mesh);
            FCharacterMeshAccessor::Access(*Character) = Mesh;

            TestNull(TEXT("no anim instance"),
                Rem::Animation::GetAnimInstance(Rem::MakeNotNull(static_cast<const AActor*>(Character))));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
