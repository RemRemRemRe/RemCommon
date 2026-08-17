// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Object/RemDrawDebugStatics.h"

#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemDrawDebugStaticsTest, "Rem.Common.DrawDebugStatics",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemDrawDebugStaticsTest::Define()
{
    Describe(TEXT("ShouldDisplayDebugForActor"), [this]
    {
        It(TEXT("should return false for a default display name"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            TestFalse(TEXT("not displayed"),
                URemDrawDebugStatics::ShouldDisplayDebugForActor(Actor, FName{TEXTVIEW("Rem.Test")}));
        });
    });

    Describe(TEXT("Draw helpers"), [this]
    {
        It(TEXT("should execute without crashing in a bare world"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            const auto Location = FVector::ZeroVector;
            const auto Rotation = FRotator::ZeroRotator;
            const auto Color = FLinearColor::Green;

            URemDrawDebugStatics::DrawHalfCircle(Actor, Location, FVector::ForwardVector,
                FVector::RightVector, 100.0f, Color);
            URemDrawDebugStatics::DrawQuarterCircle(Actor, Location, FVector::ForwardVector,
                FVector::RightVector, 100.0f, Color);
            URemDrawDebugStatics::DrawSphereAlternative(Actor, Location, Rotation, 50.0f, Color);

            TestTrue(TEXT("draw calls executed"), true);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
