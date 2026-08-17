// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemTimelinePosition.h"

#include "Enum/RemRangeFormat.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
FRemTimelinePosition_TimeAndFrame MakeTimeAndFrame(const float Time, const int32 Frame)
{
    FRemTimelinePosition_TimeAndFrame Result;
    Result.Time  = Time;
    Result.Frame = Frame;
    return Result;
}
}

DEFINE_SPEC(FRemTimelinePositionTest, "Rem.Common.TimelinePosition",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemTimelinePositionTest::Define()
{
    Describe(TEXT("FRemTimelinePosition_Time"), [this]
    {
        It(TEXT("should be within after the position time"), [this]
        {
            FRemTimelinePosition_Time Position;
            Position.Time = 1.0f;

            TestTrue(TEXT("after"), Position.IsWithinInclusive(MakeTimeAndFrame(2.0f, 0)));
            TestFalse(TEXT("before"), Position.IsWithinInclusive(MakeTimeAndFrame(0.5f, 0)));
            TestFalse(TEXT("exact time is not after"), Position.IsWithinInclusive(MakeTimeAndFrame(1.0f, 0)));
        });
    });

    Describe(TEXT("FRemTimelinePosition_Frame"), [this]
    {
        It(TEXT("should be within after the position frame"), [this]
        {
            FRemTimelinePosition_Frame Position;
            Position.Frame = 10;

            TestTrue(TEXT("after"), Position.IsWithinInclusive(MakeTimeAndFrame(0.0f, 11)));
            TestFalse(TEXT("before"), Position.IsWithinInclusive(MakeTimeAndFrame(0.0f, 9)));
            TestFalse(TEXT("exact frame is not after"), Position.IsWithinInclusive(MakeTimeAndFrame(0.0f, 10)));
        });
    });

    Describe(TEXT("FRemTimelinePosition_TimeAndFrame"), [this]
    {
        It(TEXT("should match both the time and the frame inclusively"), [this]
        {
            FRemTimelinePosition_TimeAndFrame Position;
            Position.Time  = 2.0f;
            Position.Frame = 20;

            TestTrue(TEXT("exact match"), Position.IsWithinInclusive(MakeTimeAndFrame(2.0f, 20)));
            TestFalse(TEXT("time differs"), Position.IsWithinInclusive(MakeTimeAndFrame(2.5f, 20)));
            TestFalse(TEXT("frame differs"), Position.IsWithinInclusive(MakeTimeAndFrame(2.0f, 21)));
        });
    });

    Describe(TEXT("FRemTimelinePositionRange_Time"), [this]
    {
        It(TEXT("should evaluate the start-and-end format"), [this]
        {
            FRemTimelinePositionRange_Time Range;
            Range.RangeFormat = ERemRangeFormat::StartAndEnd;
            Range.TimeRange   = FVector2f{1.0f, 3.0f};

            TestTrue(TEXT("inside"), Range.IsWithinInclusive(MakeTimeAndFrame(2.0f, 0)));
            TestTrue(TEXT("start inclusive"), Range.IsWithinInclusive(MakeTimeAndFrame(1.0f, 0)));
            TestTrue(TEXT("end inclusive"), Range.IsWithinInclusive(MakeTimeAndFrame(3.0f, 0)));
            TestFalse(TEXT("outside"), Range.IsWithinInclusive(MakeTimeAndFrame(4.0f, 0)));
        });

        It(TEXT("should evaluate the start-and-length format"), [this]
        {
            FRemTimelinePositionRange_Time Range;
            Range.RangeFormat = ERemRangeFormat::StartAndLength;
            Range.TimeRange   = FVector2f{1.0f, 2.0f};

            TestTrue(TEXT("inside"), Range.IsWithinInclusive(MakeTimeAndFrame(2.5f, 0)));
            TestFalse(TEXT("past the length"), Range.IsWithinInclusive(MakeTimeAndFrame(3.5f, 0)));
        });

        It(TEXT("should evaluate the length-and-end format"), [this]
        {
            FRemTimelinePositionRange_Time Range;
            Range.RangeFormat = ERemRangeFormat::LengthAndEnd;
            Range.TimeRange   = FVector2f{2.0f, 3.0f};

            TestTrue(TEXT("inside"), Range.IsWithinInclusive(MakeTimeAndFrame(2.0f, 0)));
            TestFalse(TEXT("before the length"), Range.IsWithinInclusive(MakeTimeAndFrame(0.5f, 0)));
        });
    });

    Describe(TEXT("FRemTimelinePositionRange_Frame"), [this]
    {
        It(TEXT("should evaluate the frame range formats"), [this]
        {
            FRemTimelinePositionRange_Frame StartEnd;
            StartEnd.RangeFormat = ERemRangeFormat::StartAndEnd;
            StartEnd.FrameRange  = FIntVector2{10, 20};

            TestTrue(TEXT("inside"), StartEnd.IsWithinInclusive(MakeTimeAndFrame(0.0f, 15)));
            TestFalse(TEXT("outside"), StartEnd.IsWithinInclusive(MakeTimeAndFrame(0.0f, 25)));

            FRemTimelinePositionRange_Frame StartLength;
            StartLength.RangeFormat = ERemRangeFormat::StartAndLength;
            StartLength.FrameRange  = FIntVector2{10, 5};

            TestTrue(TEXT("start length inside"), StartLength.IsWithinInclusive(MakeTimeAndFrame(0.0f, 14)));
            TestFalse(TEXT("start length outside"), StartLength.IsWithinInclusive(MakeTimeAndFrame(0.0f, 16)));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
