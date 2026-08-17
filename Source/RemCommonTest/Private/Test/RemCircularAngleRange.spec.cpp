// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Math/RemCircularAngleRange.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
void ExpectVector2Near(FAutomationTestBase& Test, const FVector2f Actual, const FVector2f Expected,
    const float Tolerance)
{
    Test.TestEqual(TEXT("X"), Actual.X, Expected.X, Tolerance);
    Test.TestEqual(TEXT("Y"), Actual.Y, Expected.Y, Tolerance);
}
}

DEFINE_SPEC(FRemCircularAngleRangeTest, "Rem.Common.CircularAngleRange",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemCircularAngleRangeTest::Define()
{
    Describe(TEXT("Construction"), [this]
    {
        It(TEXT("center and span should produce a non-wrapping range when it does not cross zero"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestEqual(TEXT("start"), Range.GetStartAngle(), 45.0f, 1e-4f);
            TestEqual(TEXT("end"), Range.GetEndAngle(), 135.0f, 1e-4f);
            TestFalse(TEXT("not wrapping"), Range.IsWrappingZero());
            TestFalse(TEXT("not full circle"), Range.IsFullCircle());
            TestEqual(TEXT("span"), Range.GetSpan(), 90.0f, 1e-4f);
        });

        It(TEXT("center and span should produce a wrapping range when it crosses zero"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 90.0f};

            TestEqual(TEXT("start"), Range.GetStartAngle(), 315.0f, 1e-4f);
            TestEqual(TEXT("end"), Range.GetEndAngle(), 45.0f, 1e-4f);
            TestTrue(TEXT("wrapping"), Range.IsWrappingZero());
            TestEqual(TEXT("span"), Range.GetSpan(), 90.0f, 1e-4f);
        });

        It(TEXT("full span should produce a full circle"), [this]
        {
            FRemCircularAngleRange Range{10.0f, 360.0f};

            TestTrue(TEXT("full circle"), Range.IsFullCircle());
            TestTrue(TEXT("wrapping"), Range.IsWrappingZero());
            TestEqual(TEXT("span"), Range.GetSpan(), 360.0f, 1e-4f);
        });

        It(TEXT("angles constructor should respect the clockwise flag"), [this]
        {
            FRemCircularAngleRange Clockwise;
            Clockwise.SetRangeFromAngles(45.0f, 135.0f, true);
            TestEqual(TEXT("clockwise start"), Clockwise.GetStartAngle(), 45.0f, 1e-4f);
            TestEqual(TEXT("clockwise end"), Clockwise.GetEndAngle(), 135.0f, 1e-4f);

            FRemCircularAngleRange CounterClockwise;
            CounterClockwise.SetRangeFromAngles(45.0f, 135.0f, false);
            TestEqual(TEXT("counter-clockwise start"), CounterClockwise.GetStartAngle(), 135.0f, 1e-4f);
            TestEqual(TEXT("counter-clockwise end"), CounterClockwise.GetEndAngle(), 45.0f, 1e-4f);
        });
    });

    Describe(TEXT("Contains"), [this]
    {
        It(TEXT("should test membership in a non-wrapping range"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestTrue(TEXT("inside"), Range.Contains(90.0f));
            TestTrue(TEXT("start boundary"), Range.Contains(45.0f));
            TestTrue(TEXT("end boundary"), Range.Contains(135.0f));
            TestFalse(TEXT("outside below"), Range.Contains(44.0f));
            TestFalse(TEXT("outside above"), Range.Contains(136.0f));
        });

        It(TEXT("should test membership in a wrapping range"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 90.0f};

            TestTrue(TEXT("inside high"), Range.Contains(350.0f));
            TestTrue(TEXT("inside low"), Range.Contains(10.0f));
            TestTrue(TEXT("start boundary"), Range.Contains(315.0f));
            TestTrue(TEXT("end boundary"), Range.Contains(45.0f));
            TestFalse(TEXT("gap"), Range.Contains(180.0f));
        });

        It(TEXT("should contain everything in a full circle"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 360.0f};

            TestTrue(TEXT("any angle"), Range.Contains(123.0f));
            TestTrue(TEXT("zero"), Range.Contains(0.0f));
        });
    });

    Describe(TEXT("ClampAngle"), [this]
    {
        It(TEXT("should keep angles inside the range"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestEqual(TEXT("inside"), Range.ClampAngle(90.0f), 90.0f, 1e-4f);
        });

        It(TEXT("should clamp to the nearest boundary"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestEqual(TEXT("near start"), Range.ClampAngle(44.0f), 45.0f, 1e-4f);
            TestEqual(TEXT("near end"), Range.ClampAngle(136.0f), 135.0f, 1e-4f);
        });

        It(TEXT("should clamp to the nearest boundary across zero"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 90.0f};

            TestEqual(TEXT("near end across zero"), Range.ClampAngle(46.0f), 45.0f, 1e-4f);
            TestEqual(TEXT("near start across zero"), Range.ClampAngle(314.0f), 315.0f, 1e-4f);
        });

        It(TEXT("should keep angles in a full circle"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 360.0f};

            TestEqual(TEXT("any angle"), Range.ClampAngle(200.0f), 200.0f, 1e-4f);
        });
    });

    Describe(TEXT("GetDistanceToBoundary"), [this]
    {
        It(TEXT("should measure the distance to the start boundary"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestEqual(TEXT("inside to start"), Range.GetDistanceToBoundary(90.0f, true), 45.0f, 1e-4f);
            TestEqual(TEXT("outside to start"), Range.GetDistanceToBoundary(180.0f, true), 135.0f, 1e-4f);
        });

        It(TEXT("should measure the distance to the end boundary"), [this]
        {
            FRemCircularAngleRange Range{90.0f, 90.0f};

            TestEqual(TEXT("inside to end"), Range.GetDistanceToBoundary(90.0f, false), 45.0f, 1e-4f);
            TestEqual(TEXT("outside to end"), Range.GetDistanceToBoundary(0.0f, false), 135.0f, 1e-4f);
        });

        It(TEXT("should return 360 for a full circle"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 360.0f};

            TestEqual(TEXT("start"), Range.GetDistanceToBoundary(10.0f, true), 360.0f, 1e-4f);
            TestEqual(TEXT("end"), Range.GetDistanceToBoundary(10.0f, false), 360.0f, 1e-4f);
        });
    });

    Describe(TEXT("Shortest angle"), [this]
    {
        It(TEXT("GetShortestAngleDistance should always return [0, 180]"), [this]
        {
            TestEqual(TEXT("same"), FRemCircularAngleRange::GetShortestAngleDistance(0.0f, 0.0f), 0.0f, 1e-4f);
            TestEqual(TEXT("quarter"), FRemCircularAngleRange::GetShortestAngleDistance(0.0f, 90.0f), 90.0f,
                1e-4f);
            TestEqual(TEXT("half"), FRemCircularAngleRange::GetShortestAngleDistance(0.0f, 180.0f), 180.0f,
                1e-4f);
            TestEqual(TEXT("three quarter"), FRemCircularAngleRange::GetShortestAngleDistance(0.0f, 270.0f),
                90.0f, 1e-4f);
        });

        It(TEXT("GetShortestAngle should return the shortest signed representation"), [this]
        {
            TestEqual(TEXT("360"), FRemCircularAngleRange::GetShortestAngle(360.0f), 0.0f, 1e-4f);
            TestEqual(TEXT("270"), FRemCircularAngleRange::GetShortestAngle(270.0f), -90.0f, 1e-4f);
            TestEqual(TEXT("90"), FRemCircularAngleRange::GetShortestAngle(90.0f), 90.0f, 1e-4f);
        });

        It(TEXT("IsFullCircleByAngles should detect a full circle from raw angles"), [this]
        {
            TestTrue(TEXT("0 and 360"), FRemCircularAngleRange::IsFullCircleByAngles(0.0f, 360.0f));
            TestTrue(TEXT("-180 and 180"), FRemCircularAngleRange::IsFullCircleByAngles(-180.0f, 180.0f));
            TestFalse(TEXT("10 and 20"), FRemCircularAngleRange::IsFullCircleByAngles(10.0f, 20.0f));
        });
    });

    Describe(TEXT("Center and bounds"), [this]
    {
        It(TEXT("should compute the center of wrapping and non-wrapping ranges"), [this]
        {
            FRemCircularAngleRange NonWrapping{90.0f, 90.0f};
            TestEqual(TEXT("non-wrapping center"), NonWrapping.GetCenter(), 90.0f, 1e-4f);

            FRemCircularAngleRange Wrapping{0.0f, 90.0f};
            TestEqual(TEXT("wrapping center"), Wrapping.GetCenter(), 0.0f, 1e-4f);
        });

        It(TEXT("GetBounds should return the normalized start and end"), [this]
        {
            FRemCircularAngleRange Range{0.0f, 90.0f};

            ExpectVector2Near(*this, Range.GetBounds(), FVector2f{315.0f, 45.0f}, 1e-4f);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
