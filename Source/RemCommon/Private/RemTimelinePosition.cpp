// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "RemTimelinePosition.h"

#include "Enum/RemRangeFormat.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemMacroUtilities.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemTimelinePosition)

bool FRemTimelinePosition_TimeAndFrame::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	return TimeAndFrame.Frame == Frame && FMath::IsNearlyEqual(TimeAndFrame.Time, Time);
}

bool FRemTimelinePositionRange_Time::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	switch (RangeFormat)
	{
	case ERemRangeFormat::StartAndEnd:
		return FMath::IsWithinInclusive(TimeAndFrame.Time, TimeRange.X, TimeRange.Y);
	case ERemRangeFormat::StartAndLength:
		return FMath::IsWithinInclusive(TimeAndFrame.Time, TimeRange.X, TimeRange.X + TimeRange.Y);
	case ERemRangeFormat::LengthAndEnd:
		return FMath::IsWithinInclusive(TimeAndFrame.Time, TimeRange.Y - TimeRange.X, TimeRange.Y);
	}

#if REM_WITH_DEVELOPMENT_ONLY_CODE

	RemCheckCondition(false);

#endif

	return Super::IsWithinInclusive(TimeAndFrame);
}

bool FRemTimelinePositionRange_Frame::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	switch (RangeFormat)
	{
	case ERemRangeFormat::StartAndEnd:
		return FMath::IsWithinInclusive(TimeAndFrame.Frame, FrameRange.X, FrameRange.Y);
	case ERemRangeFormat::StartAndLength:
		return FMath::IsWithinInclusive(TimeAndFrame.Frame, FrameRange.X, FrameRange.X + FrameRange.Y);
	case ERemRangeFormat::LengthAndEnd:
		return FMath::IsWithinInclusive(TimeAndFrame.Frame, FrameRange.Y - FrameRange.X, FrameRange.Y);
	}

#if REM_WITH_DEVELOPMENT_ONLY_CODE

	RemCheckCondition(false);

#endif

	return Super::IsWithinInclusive(TimeAndFrame);
}
