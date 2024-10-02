// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemTimelinePosition.generated.h"

enum class ERemRangeFormat : uint8;

struct FRemTimelinePosition_TimeAndFrame;

USTRUCT(BlueprintType, meta = (Hidden))
struct REMCOMMON_API FRemTimelinePositionBase
{
	GENERATED_BODY()

	virtual ~FRemTimelinePositionBase() = default;

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const;
};



USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTimelinePosition_Time : public FRemTimelinePositionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float Time{0.0f};

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const override;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTimelinePosition_Frame : public FRemTimelinePositionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 Frame{0};

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const override;
};


USTRUCT(BlueprintType, meta = (Hidden))
struct REMCOMMON_API FRemTimelinePosition_TimeAndFrame : public FRemTimelinePositionBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	float Time{0.0f};

	UPROPERTY(EditAnywhere, Category = "Parameter")
	int32 Frame{0};

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const override;
};



USTRUCT(BlueprintType, meta = (Hidden))
struct REMCOMMON_API FRemTimelinePositionRangeBase : public FRemTimelinePositionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTimelinePositionRange_Time : public FRemTimelinePositionRangeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	ERemRangeFormat RangeFormat{};

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FVector2f TimeRange{FVector2f::ZeroVector};

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const override;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTimelinePositionRange_Frame : public FRemTimelinePositionRangeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	ERemRangeFormat RangeFormat{};

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FIntVector2 FrameRange{FIntVector2::ZeroValue};

	virtual bool IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const override;
};

inline bool FRemTimelinePositionBase::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	return false;
}

inline bool FRemTimelinePosition_Time::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	return TimeAndFrame.Time > Time;
}

inline bool FRemTimelinePosition_Frame::IsWithinInclusive(const FRemTimelinePosition_TimeAndFrame& TimeAndFrame) const
{
	return TimeAndFrame.Frame > Frame;
}
