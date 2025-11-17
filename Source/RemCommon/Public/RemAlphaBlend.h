// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemCommonWrapperTypes.h"

#include "RemAlphaBlend.generated.h"

enum class EAlphaBlendOption : uint8;
class UCurveBase;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemAlphaBlend
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend", meta = (ClampMin = "0", ForceUnits = "s"))
	float BlendTime{1.0f};
};

USTRUCT(Blueprintable)
struct REMCOMMON_API FRemAlphaBlendOption : public FRemAlphaBlend
{
	GENERATED_BODY()

	/** Type of blending used (Linear, Cubic, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend")
	EAlphaBlendOption BlendOption{};
};

USTRUCT(Blueprintable)
struct REMCOMMON_API FRemAlphaBlendCurve : public FRemAlphaBlend
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend")
	FRemCurveBaseWrapper Curve;
};

/**
 * It has the same size as FRemAlphaBlendCurve, but could serve as an all-in-one alternative where selection by type is not needed
 */
USTRUCT(Blueprintable)
struct REMCOMMON_API FRemAlphaBlendCurveAndOption : public FRemAlphaBlendOption
{
	GENERATED_BODY()

	/**
	 * Curve used when BlendOption set to Custom
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blend")
	FRemCurveBaseWrapper Curve;

	static FVector3f GetAlpha(float Time, float BlendTime, EAlphaBlendOption BlendOption,
		const FRemCurveBaseWrapper& CurveWrapper, bool& bOutVectorAlpha);
};
