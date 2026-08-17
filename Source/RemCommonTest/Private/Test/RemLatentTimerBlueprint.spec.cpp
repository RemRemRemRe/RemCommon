// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Latent/RemLatentTimerBlueprint.h"

#include "Latent/RemLatentTimer.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemLatentTimerBlueprintTest, "Rem.Common.LatentTimerBlueprint",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemLatentTimerBlueprintTest::Define()
{
    Describe(TEXT("FRemTimerParameterBlueprintHelper_Time"), [this]
    {
        It(TEXT("should default to firing this frame without looping"), [this]
        {
            constexpr FRemTimerParameterBlueprintHelper_Time Parameter{};

            TestEqual(TEXT("delay"), Parameter.TimeToDelay, 0.0f);
            TestEqual(TEXT("loop count"), Parameter.LoopCount, 1);
            TestEqual(TEXT("initial delay unset"), Parameter.InitialDelay, -1.0f);
            TestFalse(TEXT("max once per frame"), Parameter.bMaxOncePerFrame);
        });

        It(TEXT("operator* should convert to the latent helper"), [this]
        {
            FRemTimerParameterBlueprintHelper_Time Parameter;
            Parameter.TimeToDelay = 0.25f;
            Parameter.LoopCount = 3;

            const auto Converted = *Parameter;

            TestEqual(TEXT("delay"), Converted.TimeToDelay, 0.25f);
            TestEqual(TEXT("loop count"), static_cast<int32>(Converted.LoopCount), 3);
        });
    });

    Describe(TEXT("FRemTimerParameterBlueprintHelper_Frame"), [this]
    {
        It(TEXT("should default to firing this frame without looping"), [this]
        {
            constexpr FRemTimerParameterBlueprintHelper_Frame Parameter{};

            TestEqual(TEXT("frame delay"), Parameter.FrameToDelay, 0);
            TestEqual(TEXT("loop count"), Parameter.LoopCount, 1);
            TestEqual(TEXT("initial delay sentinel"), Parameter.InitialDelay,
                std::numeric_limits<int32>::max());
        });

        It(TEXT("operator* should map the unset sentinel to the latent sentinel"), [this]
        {
            constexpr FRemTimerParameterBlueprintHelper_Frame Parameter{};

            const auto Converted = *Parameter;

            // the blueprint sentinel int32::max() must map to the latent uint32::max()
            // or the default initial delay would be ~2^31 frames
            TestEqual(TEXT("sentinel mapped"), static_cast<int32>(Converted.InitialDelay),
                static_cast<int32>(std::numeric_limits<uint32>::max()));
        });

        It(TEXT("operator* should pass an explicit initial delay through"), [this]
        {
            FRemTimerParameterBlueprintHelper_Frame Parameter;
            Parameter.FrameToDelay = 2;
            Parameter.InitialDelay = 4;

            const auto Converted = *Parameter;

            TestEqual(TEXT("frame delay"), static_cast<int32>(Converted.FrameToDelay), 2);
            TestEqual(TEXT("initial delay"), static_cast<int32>(Converted.InitialDelay), 4);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
