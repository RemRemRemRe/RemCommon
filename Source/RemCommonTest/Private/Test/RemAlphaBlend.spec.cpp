// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemAlphaBlend.h"

#include "AlphaBlend.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemAlphaBlendTest, "Rem.Common.AlphaBlend",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemAlphaBlendTest::Define()
{
    Describe(TEXT("Defaults"), [this]
    {
        It(TEXT("should default to a one second blend"), [this]
        {
            constexpr FRemAlphaBlend Blend{};

            TestEqual(TEXT("blend time"), Blend.BlendTime, 1.0f);
        });

        It(TEXT("option and curve variants should derive from the base"), [this]
        {
            TestTrue(TEXT("option is child"),
                FRemAlphaBlendOption::StaticStruct()->IsChildOf(FRemAlphaBlend::StaticStruct()));
            TestTrue(TEXT("curve is child"),
                FRemAlphaBlendCurve::StaticStruct()->IsChildOf(FRemAlphaBlend::StaticStruct()));
            TestTrue(TEXT("curve and option is child"),
                FRemAlphaBlendCurveAndOption::StaticStruct()->IsChildOf(FRemAlphaBlendOption::StaticStruct()));
        });
    });

    Describe(TEXT("GetAlpha"), [this]
    {
        It(TEXT("should clamp and evaluate a linear blend"), [this]
        {
            auto bVectorAlpha = true;

            const auto Half = FRemAlphaBlendCurveAndOption::GetAlpha(
                0.5f, 1.0f, EAlphaBlendOption::Linear, FRemCurveBaseWrapper{}, bVectorAlpha);

            TestFalse(TEXT("scalar alpha"), bVectorAlpha);
            TestEqual(TEXT("half"), Half.X, 0.5f, 1e-4f);
        });

        It(TEXT("should clamp beyond the blend time"), [this]
        {
            auto bVectorAlpha = true;

            const auto Clamped = FRemAlphaBlendCurveAndOption::GetAlpha(
                2.0f, 1.0f, EAlphaBlendOption::Linear, FRemCurveBaseWrapper{}, bVectorAlpha);

            TestEqual(TEXT("clamped to one"), Clamped.X, 1.0f, 1e-4f);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
