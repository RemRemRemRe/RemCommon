// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Latent/RemLatentTimer.h"

#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemLatentTimerTest, "Rem.Common.LatentTimer",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemLatentTimerTest::Define()
{
    Describe(TEXT("FTimerHandle"), [this]
    {
        It(TEXT("zero should mean an invalid handle"), [this]
        {
            const Rem::Latent::FTimerHandle Handle;

            TestFalse(TEXT("invalid"), Handle.IsValid());
        });

        It(TEXT("a new handle should be valid and unique"), [this]
        {
            const auto First  = Rem::Latent::FTimerHandle::NewHandle();
            const auto Second = Rem::Latent::FTimerHandle::NewHandle();

            TestTrue(TEXT("first valid"), First.IsValid());
            TestTrue(TEXT("second valid"), Second.IsValid());
            TestFalse(TEXT("unique"), First.Handle == Second.Handle);
        });
    });

    Describe(TEXT("SetTimerForThisTick"), [this]
    {
        It(TEXT("should fire the delegate on the next tick"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            int32 CallCount = 0;
            Rem::Latent::FTimerHandle Handle = Rem::Latent::SetTimerForThisTick(
                Rem::MakeNotNull(Object), Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }));

            TestTrue(TEXT("handle valid"), Handle.IsValid());
            TestEqual(TEXT("not fired yet"), CallCount, 0);

            World.Tick(0.1f);

            TestEqual(TEXT("fired once"), CallCount, 1);
        });
    });

    Describe(TEXT("SetTimerForNextTick"), [this]
    {
        It(TEXT("should fire the delegate on the following tick"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            int32 CallCount = 0;
            Rem::Latent::SetTimerForNextTick(
                Rem::MakeNotNull(Object), Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }));

            World.Tick(0.1f);
            TestEqual(TEXT("not fired on the registration tick"), CallCount, 0);

            World.Tick(0.1f);
            TestEqual(TEXT("fired on the next tick"), CallCount, 1);
        });
    });

    Describe(TEXT("SetTimer with time delay"), [this]
    {
        It(TEXT("should fire after the initial delay"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            Rem::Latent::FTimerParameterHelper_Time Parameter;
            Parameter.TimeToDelay  = 0.5f;
            Parameter.InitialDelay = 0.1f;

            int32 CallCount = 0;
            Rem::Latent::SetTimer(Rem::MakeNotNull(Object),
                Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }), Parameter);

            World.Tick(0.05f);
            TestEqual(TEXT("not fired yet"), CallCount, 0);

            World.Tick(0.1f);
            TestEqual(TEXT("fired after the initial delay"), CallCount, 1);
        });

        It(TEXT("should loop the configured number of times"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            Rem::Latent::FTimerParameterHelper_Time Parameter;
            Parameter.TimeToDelay = 0.0f;
            Parameter.LoopCount = 3;

            int32 CallCount = 0;
            Rem::Latent::SetTimer(Rem::MakeNotNull(Object),
                Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }), Parameter);

            World.Tick(0.1f);
            World.Tick(0.1f);
            World.Tick(0.1f);

            TestEqual(TEXT("fired three times"), CallCount, 3);
        });
    });

    Describe(TEXT("Timer control"), [this]
    {
        It(TEXT("should stop and report inactive timers"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            Rem::Latent::FTimerParameterHelper_Time Parameter;
            Parameter.TimeToDelay = 1.0f;

            int32 CallCount = 0;
            auto Handle = Rem::Latent::SetTimer(Rem::MakeNotNull(Object),
                Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }), Parameter);

            TestTrue(TEXT("active"), Rem::Latent::IsTimerActive(Rem::MakeNotNull(Object), Handle));

            Rem::Latent::StopTimer(Rem::MakeNotNull(Object), Handle);

            TestFalse(TEXT("handle invalidated"), Handle.IsValid());
            TestFalse(TEXT("inactive"), Rem::Latent::IsTimerActive(Rem::MakeNotNull(Object), Handle));

            World.Tick(2.0f);

            TestEqual(TEXT("never fired"), CallCount, 0);
        });

        It(TEXT("should pause and unpause a timer"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            Rem::Latent::FTimerParameterHelper_Time Parameter;
            Parameter.TimeToDelay = 0.3f;

            int32 CallCount = 0;
            const auto Handle = Rem::Latent::SetTimer(Rem::MakeNotNull(Object),
                Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }), Parameter);

            Rem::Latent::PauseTimer(Rem::MakeNotNull(Object), Handle);

            World.Tick(1.0f);
            TestEqual(TEXT("paused"), CallCount, 0);

            Rem::Latent::UnpauseTimer(Rem::MakeNotNull(Object), Handle);

            World.Tick(0.5f);
            TestEqual(TEXT("fired after unpause"), CallCount, 1);
        });

        It(TEXT("should reset the delay of an active timer"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Object = World->SpawnActor<AActor>();
            check(Object);

            Rem::Latent::FTimerParameterHelper_Time Parameter;
            Parameter.TimeToDelay = 0.2f;

            int32 CallCount = 0;
            const auto Handle = Rem::Latent::SetTimer(Rem::MakeNotNull(Object),
                Rem::FTimerDelegate::CreateLambda([&CallCount] { ++CallCount; }), Parameter);

            World.Tick(0.1f);
            TestTrue(TEXT("reset succeeds"), Rem::Latent::ResetTimerDelay(Rem::MakeNotNull(Object), Handle));

            World.Tick(0.1f);
            TestEqual(TEXT("not fired after reset"), CallCount, 0);

            World.Tick(0.2f);
            TestEqual(TEXT("fired after the reset delay"), CallCount, 1);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
