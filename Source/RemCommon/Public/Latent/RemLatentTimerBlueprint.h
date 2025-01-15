// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemLatentTimerBlueprint.generated.h"

namespace Rem::Latent
{
	struct FTimerParameterHelper_Time;
	struct FTimerParameterHelper_Frame;
}

/**
 * by default, it set to execute this frame, no loop, no initial delay, up to call once per frame
 *
 * @LoopCount 0 means loop forever
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FTimerParameterBlueprintHelper_Time
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "s", ClampMin = 0))
	float TimeToDelay{0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "times", ClampMin = 0))
	int64 LoopCount{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "s", ClampMin = 0))
	float InitialDelay{-1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	bool bMaxOncePerFrame{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	bool bSkipCountingThisFrame{false};

	Rem::Latent::FTimerParameterHelper_Time operator*() const;
};

/**
 * by default, it set to execute this frame, no loop, no initial delay, up to call once per frame
 *
 * @LoopCount 0 means loop forever
 *
 * @note the frame format could only be called up to once per frame
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FTimerParameterBlueprintHelper_Frame
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "frame", ClampMin = 0))
	int64 FrameToDelay{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "times", ClampMin = 0))
	int64 LoopCount{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (ForceUnits = "frame", ClampMin = 0))
	int64 InitialDelay{std::numeric_limits<uint32>::max()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	bool bSkipCountingThisFrame{false};

	Rem::Latent::FTimerParameterHelper_Frame operator*() const;
};
