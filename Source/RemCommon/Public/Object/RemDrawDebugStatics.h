// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemDrawDebugStatics.generated.h"

UCLASS()
class REMCOMMON_API URemDrawDebugStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

namespace Rem::DrawDebug
{
	/**
	 * Draw a crosshair on the screen. By default, it draw the crosshair at center of the screen
	 */
	REMCOMMON_API void DrawDebugCrossHair(const UWorld& World, float CrossHairLineLength = 20.0f,
			float LineThickness = 2.f, float AngleToRotate = 0.0f, const FVector2f& CrossHairCenterScreenSpace = {-1.0f, -1.0f},
		const FLinearColor& LineColor = FLinearColor::White);
}
