// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemAlphaBlend.generated.h"

enum class EAlphaBlendOption : uint8;
class UCurveBase;

USTRUCT(BlueprintType)
struct FRemAlphaBlend
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Blend", meta = (ClampMin = "0"))
	float BlendTime{1.0f};
};

USTRUCT(Blueprintable)
struct FRemAlphaBlendOption : public FRemAlphaBlend
{
	GENERATED_BODY()
	
	/** Type of blending used (Linear, Cubic, etc.) */
	UPROPERTY(EditAnywhere, Category = "Blend")
	EAlphaBlendOption BlendOption{};
};

USTRUCT(Blueprintable)
struct FRemAlphaBlendCurve : public FRemAlphaBlend
{
	GENERATED_BODY()
	
	/** Type of blending used (Linear, Cubic, etc.) */
	UPROPERTY(EditAnywhere, Category = "Blend", meta = (AllowedClasses = "/Script/Engine.CurveFloat, /Script/Engine.CurveVector"))
	TObjectPtr<UCurveBase> Curve;
};
