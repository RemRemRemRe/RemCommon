// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Math/RemMath.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Math::DamperExact;
using Rem::Math::Lerp;
using Rem::Math::CombineNumericRanges;
using Rem::Math::Clamp01;
using Rem::Math::Saturate;
using Rem::Math::RemapAngleForCounterClockwiseRotation;
using Rem::Math::RadianToDirectionXY;
using Rem::Math::AngleToDirectionXY;
using Rem::Math::PerpendicularClockwiseXY;
using Rem::Math::PerpendicularCounterClockwiseXY;
using Rem::Math::GetWrappedValueInRange;

void ExpectVector3Near(FAutomationTestBase& Test, const FVector3f Actual, const FVector3f Expected,
    const float Tolerance)
{
    Test.TestEqual(TEXT("X"), Actual.X, Expected.X, Tolerance);
    Test.TestEqual(TEXT("Y"), Actual.Y, Expected.Y, Tolerance);
    Test.TestEqual(TEXT("Z"), Actual.Z, Expected.Z, Tolerance);
}
}

DEFINE_SPEC(FRemMathTest, "Rem.Common.Math",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemMathTest::Define()
{
    Describe(TEXT("DamperExact scalar"), [this]
    {
        It(TEXT("should move roughly half the distance after one half-life"), [this]
        {
            TestEqual(TEXT("factor"), DamperExact(1.0f, 1.0f), 0.5f, 0.05f);
        });

        It(TEXT("should snap to the target with zero half-life"), [this]
        {
            TestEqual(TEXT("factor"), DamperExact(0.1f, 0.0f), 1.0f, 1e-3f);
        });

        It(TEXT("should approach 1 asymptotically with more time"), [this]
        {
            // InvExpApprox error grows with large exponents; keep a loose bound
            TestEqual(TEXT("factor"), DamperExact(10.0f, 1.0f), 1.0f, 0.01f);
        });
    });

    Describe(TEXT("DamperExact typed"), [this]
    {
        It(TEXT("should interpolate a vector with the scalar half-life"), [this]
        {
            const auto Result = DamperExact(FVector3f::ZeroVector, FVector3f{10.0f, 0.0f, 0.0f}, 1.0f, 1.0f);

            TestEqual(TEXT("X"), Result.X, 5.0f, 0.05f);
        });

        It(TEXT("should return the target when current equals target"), [this]
        {
            const auto Result = DamperExact(FVector3f{3.0f, 4.0f, 5.0f}, FVector3f{3.0f, 4.0f, 5.0f}, 1.0f, 1.0f);

            ExpectVector3Near(*this, Result, FVector3f{3.0f, 4.0f, 5.0f}, 1e-4f);
        });

        It(TEXT("should interpolate a vector per-axis with a half-life vector"), [this]
        {
            const auto Result = DamperExact(FVector3f::ZeroVector, FVector3f{10.0f, 20.0f, 30.0f}, 1.0f,
                FVector3f{1.0f, 2.0f, 0.5f});

            TestEqual(TEXT("X after one half-life"), Result.X, 5.0f, 0.05f);
            TestEqual(TEXT("Y after one half-life"), Result.Y, 5.858f, 0.05f);
            // InvExpApprox error grows for short half-lives; keep a loose bound
            TestEqual(TEXT("Z after one half-life"), Result.Z, 22.5f, 0.2f);
        });

        It(TEXT("should interpolate a rotator toward the target"), [this]
        {
            const auto Result = DamperExact(FRotator3f::ZeroRotator, FRotator3f{10.0f, 0.0f, 0.0f}, 1.0f, 1.0f);

            TestEqual(TEXT("pitch"), Result.Pitch, 5.0f, 0.05f);
        });

        It(TEXT("should interpolate a quaternion toward the target"), [this]
        {
            const auto TargetRotation = FRotator3f{0.0f, 90.0f, 0.0f}.Quaternion();
            auto Value = FQuat4f::Identity;
            for (int32 Frame = 0; Frame < 200; ++Frame)
            {
                Value = DamperExact(Value, TargetRotation, 0.1f, 1.0f);
            }

            TestEqual(TEXT("yaw after convergence"), Value.Rotator().Yaw, 90.0f, 1.0f);
        });
    });

    Describe(TEXT("Lerp"), [this]
    {
        It(TEXT("should lerp a vector"), [this]
        {
            const auto Result = Lerp(FVector3f::ZeroVector, FVector3f{10.0f, 20.0f, 30.0f}, 0.5f);

            ExpectVector3Near(*this, Result, FVector3f{5.0f, 10.0f, 15.0f}, 1e-4f);
        });

        It(TEXT("should lerp a rotator along the shortest path"), [this]
        {
            // 350 -> 10 through 0 is a 20 degree step
            const auto Result = Lerp(FRotator3f{0.0f, 350.0f, 0.0f}, FRotator3f{0.0f, 10.0f, 0.0f}, 0.5f);

            TestEqual(TEXT("yaw"), Result.Yaw, 0.0f, 1e-3f);
        });

        It(TEXT("should slerp a quaternion"), [this]
        {
            const auto Result = Lerp(FQuat4f::Identity, FRotator3f{0.0f, 90.0f, 0.0f}.Quaternion(), 0.5f);

            TestEqual(TEXT("yaw"), Result.Rotator().Yaw, 45.0f, 1e-3f);
        });

        It(TEXT("should lerp a TAngle along the shortest path"), [this]
        {
            Rem::Math::TAngle<float> From{350.0f};
            Rem::Math::TAngle<float> To{10.0f};

            const auto Result = Lerp(From, To, 0.5f);

            TestEqual(TEXT("angle"), *Result, 0.0f, 1e-3f);
        });
    });

    Describe(TEXT("Numeric ranges"), [this]
    {
        It(TEXT("CombineNumericRanges should intersect the two ranges"), [this]
        {
            const auto Result = CombineNumericRanges(-90.0f, -45.0f, 90.0f, 45.0f);

            TestEqual(TEXT("min"), Result.X, -45.0f, 1e-4f);
            TestEqual(TEXT("max"), Result.Y, 45.0f, 1e-4f);
        });

        It(TEXT("CombineNumericRanges should produce an inverted range when disjoint"), [this]
        {
            const auto Result = CombineNumericRanges(0.0f, 90.0f, 45.0f, 180.0f);

            TestEqual(TEXT("min"), Result.X, 90.0f, 1e-4f);
            TestEqual(TEXT("max"), Result.Y, 45.0f, 1e-4f);
        });
    });

    Describe(TEXT("Clamp01 and Saturate"), [this]
    {
        It(TEXT("should clamp values into [0, 1]"), [this]
        {
            TestEqual(TEXT("below"), Clamp01(-0.5f), 0.0f, 1e-6f);
            TestEqual(TEXT("inside"), Clamp01(0.5f), 0.5f, 1e-6f);
            TestEqual(TEXT("above"), Clamp01(1.5f), 1.0f, 1e-6f);
        });

        It(TEXT("Saturate should match Clamp01"), [this]
        {
            TestEqual(TEXT("below"), Saturate(-0.5f), Clamp01(-0.5f), 1e-6f);
            TestEqual(TEXT("above"), Saturate(1.5f), Clamp01(1.5f), 1e-6f);
        });
    });

    Describe(TEXT("Angle helpers"), [this]
    {
        It(TEXT("RemapAngleForCounterClockwiseRotation should remap angles above 180"), [this]
        {
            TestEqual(TEXT("181 remaps"), RemapAngleForCounterClockwiseRotation(181.0f), -179.0f, 1e-4f);
            TestEqual(TEXT("180 remaps too"), RemapAngleForCounterClockwiseRotation(180.0f), -180.0f, 1e-4f);
            TestEqual(TEXT("90 stays"), RemapAngleForCounterClockwiseRotation(90.0f), 90.0f, 1e-4f);
        });

        It(TEXT("AngleToDirectionXY should map angles to unit vectors"), [this]
        {
            ExpectVector3Near(*this, FVector3f{AngleToDirectionXY(0.0f), 0.0f}, FVector3f{1.0f, 0.0f, 0.0f}, 1e-4f);
            ExpectVector3Near(*this, FVector3f{AngleToDirectionXY(90.0f), 0.0f}, FVector3f{0.0f, 1.0f, 0.0f}, 1e-4f);
            ExpectVector3Near(*this, FVector3f{AngleToDirectionXY(180.0f), 0.0f}, FVector3f{-1.0f, 0.0f, 0.0f},
                1e-4f);
        });

        It(TEXT("RadianToDirectionXY should map radians to unit vectors"), [this]
        {
            ExpectVector3Near(*this, FVector3f{RadianToDirectionXY(0.0f), 0.0f}, FVector3f{1.0f, 0.0f, 0.0f}, 1e-4f);
            ExpectVector3Near(*this, FVector3f{RadianToDirectionXY(UE_PI * 0.5f), 0.0f}, FVector3f{0.0f, 1.0f, 0.0f},
                1e-4f);
        });
    });

    Describe(TEXT("Perpendicular"), [this]
    {
        It(TEXT("PerpendicularClockwiseXY should rotate clockwise"), [this]
        {
            const auto Result = PerpendicularClockwiseXY(FVector2f{1.0f, 0.0f});

            TestEqual(TEXT("X"), Result.X, 0.0f, 1e-6f);
            TestEqual(TEXT("Y"), Result.Y, -1.0f, 1e-6f);
        });

        It(TEXT("PerpendicularCounterClockwiseXY should rotate counter-clockwise"), [this]
        {
            const auto Result = PerpendicularCounterClockwiseXY(FVector2f{1.0f, 0.0f});

            TestEqual(TEXT("X"), Result.X, 0.0f, 1e-6f);
            TestEqual(TEXT("Y"), Result.Y, 1.0f, 1e-6f);
        });
    });

    Describe(TEXT("GetWrappedValueInRange"), [this]
    {
        It(TEXT("should wrap values into the range"), [this]
        {
            TestEqual(TEXT("inside stays"), GetWrappedValueInRange(10.0f, {0.0f, 360.0f}), 10.0f, 1e-4f);
            TestEqual(TEXT("above wraps"), GetWrappedValueInRange(370.0f, {0.0f, 360.0f}), 10.0f, 1e-4f);
            TestEqual(TEXT("below keeps sign"), GetWrappedValueInRange(-10.0f, {0.0f, 360.0f}), -10.0f, 1e-4f);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
