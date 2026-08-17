// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Math/RemMathCore.h"

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemMathCoreTest, "Rem.Common.MathCore",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemMathCoreTest::Define()
{
    Describe(TEXT("Type dispatched constants"), [this]
    {
        It(TEXT("GetKindSmallNumber should dispatch by floating point type"), [this]
        {
            TestTrue(TEXT("float"), FMath::IsNearlyEqual(
                static_cast<float>(Rem::Math::GetKindSmallNumber<float>()),
                static_cast<float>(UE_KINDA_SMALL_NUMBER)));
            TestTrue(TEXT("double"), FMath::IsNearlyEqual(
                static_cast<double>(Rem::Math::GetKindSmallNumber<double>()),
                static_cast<double>(UE_DOUBLE_KINDA_SMALL_NUMBER)));
        });

        It(TEXT("GetSmallNumber should dispatch by floating point type"), [this]
        {
            TestTrue(TEXT("float"), FMath::IsNearlyEqual(
                static_cast<float>(Rem::Math::GetSmallNumber<float>()),
                static_cast<float>(UE_SMALL_NUMBER)));
            TestTrue(TEXT("double"), FMath::IsNearlyEqual(
                static_cast<double>(Rem::Math::GetSmallNumber<double>()),
                static_cast<double>(UE_DOUBLE_SMALL_NUMBER)));
        });

        It(TEXT("GetPI should dispatch by floating point type"), [this]
        {
            TestTrue(TEXT("float"), FMath::IsNearlyEqual(
                static_cast<float>(Rem::Math::GetPI<float>()), static_cast<float>(UE_PI)));
            TestEqual(TEXT("double"), static_cast<double>(Rem::Math::GetPI<double>()),
                3.1415926535897931, 1e-9);
        });
    });

    Describe(TEXT("Project constants"), [this]
    {
        It(TEXT("should expose the documented thresholds"), [this]
        {
            TestEqual(TEXT("counter clockwise threshold"),
                Rem::Math::CounterClockwiseRotationAngleThreshold, 5.0f);
            TestEqual(TEXT("five digits float"),
                Rem::Math::FiveDigitsAfterDecimalPoint, 1.e-5f);
            TestEqual(TEXT("five digits double"),
                Rem::Math::FiveDigitsAfterDecimalPointDouble, 1.e-5);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
