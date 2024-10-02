// Copyright RemRemRemRe. 2024. All Rights Reserved.

#include "Object/RemDrawDebugStatics.h"

#include "Object/RemObjectStatics.inl"
#include "Engine/World.h"
#include "GameFramework/HUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemDrawDebugStatics)

namespace Rem::DrawDebug
{

void DrawDebugCrossHair(const UWorld& World, const float CrossHairLineLength, const float LineThickness,
	const float AngleToRotate, const FVector2f& CrossHairCenterScreenSpace,	const FLinearColor& LineColor)
{
	auto* HUD = Object::GetFirstLocalHUD(World);
	RemCheckVariable(HUD, return;);

	auto GetCrossHairCenter = [&]
	{
		if (CrossHairCenterScreenSpace.X < 0.0f || CrossHairCenterScreenSpace.Y < 0.0f)
		{
			auto* Controller = Object::GetFirstLocalPlayerController(World);
			RemCheckVariable(Controller, return FVector2f::ZeroVector);

			int32 SizeX, SizeY;
			Controller->GetViewportSize(SizeX, SizeY);

			const auto CenterX = static_cast<float>(SizeX / 2);
			const auto CenterY = static_cast<float>(SizeY / 2);
			return FVector2f{CenterX, CenterY};
		}

		return CrossHairCenterScreenSpace;
	};

	auto GetRotatedDirection = [&](const FVector2f& InitialDirection)
	{
		const auto Rotated = FVector3f{InitialDirection, 0.0f}
			.RotateAngleAxis(AngleToRotate, FVector3f::UpVector);

		return FVector2f{Rotated};
	};

	const auto DirectionX = FMath::IsNearlyZero(AngleToRotate)
		? FVector2f{1.0f, 0.0f}
		: GetRotatedDirection(FVector2f{1.0f, 0.0f});

	const auto DirectionY = FMath::IsNearlyZero(AngleToRotate)
		? FVector2f{0.0f, 1.0f}
		: GetRotatedDirection(FVector2f{0.0f, 1.0f});

	const auto CrossHairCenter = GetCrossHairCenter();
	auto DrawLine = [&](const FVector2f& Direction)
	{
		const auto LineStart = CrossHairCenter - Direction * CrossHairLineLength * 0.5f;
		const auto LineEnd = CrossHairCenter + Direction * CrossHairLineLength * 0.5f;

		HUD->DrawLine(LineStart.X, LineStart.Y, LineEnd.X, LineEnd.Y, LineColor, LineThickness);
	};

	DrawLine(DirectionX);
	DrawLine(DirectionY);
}

}
