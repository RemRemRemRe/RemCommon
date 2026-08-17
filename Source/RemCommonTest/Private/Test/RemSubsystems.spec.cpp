// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Subsystems/RemEngineSubsystem.inl"
#include "Subsystems/RemWorldSubsystem.inl"

#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"
#include "Subsystems/EngineSubsystem.h"
#include "Subsystems/WorldSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemSubsystemsTest, "Rem.Common.Subsystems",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemSubsystemsTest::Define()
{
    Describe(TEXT("Engine subsystem accessors"), [this]
    {
        It(TEXT("should resolve the registered engine subsystem"), [this]
        {
            // the engine registers base UEngineSubsystem instances by default:
            // the accessor must resolve them without tripping any assertion
            TestNotNull(TEXT("engine subsystem"),
                Rem::Subsystem::GetEngineSubsystem<UEngineSubsystem>());
        });
    });

    Describe(TEXT("World subsystem accessors"), [this]
    {
        It(TEXT("should resolve the registered world subsystem"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            TestNotNull(TEXT("world subsystem"),
                Rem::Subsystem::GetWorldSubsystem<UWorldSubsystem>(Rem::MakeNotNull(Actor)));
        });
    });

    // note: the game-instance / local-player / audio-engine accessors check
    // their context (RemCheckCondition) and cannot be exercised without a test
    // world variant that owns a game instance - future FRemTestWorld extension.
}

#endif // WITH_DEV_AUTOMATION_TESTS
