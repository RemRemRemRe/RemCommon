// Copyright RemRemRemRe. 2024. All Rights Reserved.

#include "Object/RemDrawDebugStatics.h"

#include "Object/RemObjectStatics.inl"
#include "DrawDebugHelpers.h"
#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemDrawDebugStatics)

using namespace Rem::DrawDebug;

bool URemDrawDebugStatics::ShouldDisplayDebugForActor(const AActor* Actor, const FName& DisplayName)
{
	const auto* World{IsValid(Actor) ? Actor->GetWorld() : nullptr};
	const auto* Player{IsValid(World) ? World->GetFirstPlayerController() : nullptr};
	auto* Hud{IsValid(Player) ? Player->GetHUD() : nullptr};

	return IsValid(Hud) && Hud->ShouldDisplayDebug(DisplayName) && Hud->GetCurrentDebugTargetActor() == Actor;
}

void URemDrawDebugStatics::DrawHalfCircle(const UObject* WorldContext, const FVector& Location, const FVector& XAxis,
	const FVector& YAxis, const float Radius, const FLinearColor& Color,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto FColor{Color.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	auto PreviousVertex{Location + XAxis * Radius};

	for (auto i{1}; i <= DrawCircleSidesCount / 2; i++)
	{
		static constexpr auto DeltaRadian{UE_TWO_PI / DrawCircleSidesCount};

		float Sin, Cos;
		FMath::SinCos(&Sin, &Cos, DeltaRadian * static_cast<float>(i));

		const auto NextVertex{Location + Radius * Cos * XAxis + Radius * Sin * YAxis};

		DrawDebugLine(World, PreviousVertex, NextVertex, FColor, bPersistent, Duration, DepthPriority, Thickness);

		PreviousVertex = NextVertex;
	}
#endif
}

void URemDrawDebugStatics::DrawQuarterCircle(const UObject* WorldContext, const FVector& Location, const FVector& XAxis,
	const FVector& YAxis, const float Radius, const FLinearColor& Color,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto FColor{Color.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	auto PreviousVertex{Location + XAxis * Radius};

	for (auto i{1}; i <= DrawCircleSidesCount / 4; i++)
	{
		static constexpr auto DeltaRadian{UE_TWO_PI / DrawCircleSidesCount};

		float Sin, Cos;
		FMath::SinCos(&Sin, &Cos, DeltaRadian * static_cast<float>(i));

		const auto NextVertex{Location + Radius * Cos * XAxis + Radius * Sin * YAxis};

		DrawDebugLine(World, PreviousVertex, NextVertex, FColor, bPersistent, Duration, DepthPriority, Thickness);

		PreviousVertex = NextVertex;
	}
#endif
}

void URemDrawDebugStatics::DrawSphereAlternative(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
	const float Radius, const FLinearColor& Color, const float Duration,
	const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto FColor{Color.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	const FRotationMatrix RotationMatrix{Rotation};

	FVector XAxis, YAxis, ZAxis;
	RotationMatrix.GetScaledAxes(XAxis, YAxis, ZAxis);

	DrawCircle(World, Location, XAxis, YAxis, FColor, Radius, DrawCircleSidesCount, bPersistent, Duration, DepthPriority, Thickness);
	DrawCircle(World, Location, XAxis, ZAxis, FColor, Radius, DrawCircleSidesCount, bPersistent, Duration, DepthPriority, Thickness);
	DrawCircle(World, Location, YAxis, ZAxis, FColor, Radius, DrawCircleSidesCount, bPersistent, Duration, DepthPriority, Thickness);
#endif
}

void URemDrawDebugStatics::DrawLineTraceSingle(const UObject* WorldContext, const FVector& Start, const FVector& End, const bool bHit,
	const FHitResult& Hit, const FLinearColor& TraceColor, const FLinearColor& HitColor,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto bPersistent{Duration < 0.0f};

	DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, Duration, DepthPriority, Thickness);

	if (bHit && Hit.bBlockingHit)
	{
		DrawDebugPoint(World, Hit.ImpactPoint, DrawImpactPointSize, HitColor.ToFColor(true), bPersistent, Duration, DepthPriority);
	}
#endif
}

void URemDrawDebugStatics::DrawSweepSphere(const UObject* WorldContext, const FVector& Start, const FVector& End, const float Radius,
	const FLinearColor& Color, const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto FColor{Color.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	const auto SweepVector{End - Start};

	DrawDebugCapsule(World, Start + SweepVector * 0.5f, UE_REAL_TO_FLOAT(SweepVector.Size()) * 0.5f + Radius,
		Radius, FRotationMatrix::MakeFromZ(SweepVector).ToQuat(),
		FColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugDirectionalArrow(World, Start, End, DrawArrowSize, FColor, bPersistent, Duration, DepthPriority, Thickness);
#endif
}

void URemDrawDebugStatics::DrawSweepSingleSphere(const UObject* WorldContext, const FVector& Start, const FVector& End,
	const float Radius, const bool bHit, const FHitResult& Hit,
	const FLinearColor& SweepColor, const FLinearColor& HitColor,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	DrawSweepSphere(World, Start, End, Radius, SweepColor.ToFColor(true), Duration, Thickness, DepthPriority);

	if (bHit && Hit.bBlockingHit)
	{
		const auto HitFColor{HitColor.ToFColor(true)};

		DrawSphereAlternative(World, Hit.Location, (End - Start).ToOrientationRotator(),
			Radius, HitFColor, Duration, Thickness, DepthPriority);

		DrawDebugPoint(World, Hit.ImpactPoint, DrawImpactPointSize, HitFColor, Duration < 0.0f, Duration, DepthPriority);
	}
#endif
}

void URemDrawDebugStatics::DrawSweepSingleCapsule(const UObject* WorldContext, const FVector& Start, const FVector& End,
	const FRotator& Rotation, const float Radius, const float HalfHeight, const bool bHit,
	const FHitResult& Hit, const FLinearColor& SweepColor, const FLinearColor& HitColor,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto SweepFColor{SweepColor.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	const auto Quaternion{Rotation.Quaternion()};

	DrawDebugCapsule(World, Start, HalfHeight, Radius, Quaternion, SweepFColor, bPersistent, Duration, DepthPriority, Thickness);
	DrawDebugCapsule(World, End, HalfHeight, Radius, Quaternion, SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugDirectionalArrow(World, Start, End, DrawArrowSize, SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	if (bHit && Hit.bBlockingHit)
	{
		const auto HitFColor{HitColor.ToFColor(true)};

		DrawDebugCapsule(World, Hit.Location, HalfHeight, Radius, Quaternion,
			HitFColor, bPersistent, Duration, DepthPriority, Thickness);

		DrawDebugPoint(World, Hit.ImpactPoint, DrawImpactPointSize, HitFColor, bPersistent, Duration, DepthPriority);
	}
#endif
}

void URemDrawDebugStatics::DrawSweepSingleCapsuleAlternative(const UObject* WorldContext, const FVector& Start, const FVector& End,
	const float Radius, const float HalfHeight, const bool bHit, const FHitResult& Hit,
	const FLinearColor& SweepColor, const FLinearColor& HitColor,
	const float Duration, const float Thickness, const uint8 DepthPriority)
{
#if ENABLE_DRAW_DEBUG
	const auto* World{IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr};
	RemCheckVariable(World, return;);

	const auto SweepFColor{SweepColor.ToFColor(true)};
	const auto bPersistent{Duration < 0.0f};

	const auto Rotation{(End - Start).ToOrientationQuat()};
	const FQuatRotationMatrix RotationMatrix{Rotation};

	FVector XAxis, YAxis, ZAxis;
	RotationMatrix.GetScaledAxes(XAxis, YAxis, ZAxis);

	const auto XAxisInverse{-XAxis};
	const auto ZAxisInverse{-ZAxis};

	const auto DistanceToHemisphere{FMath::Max(1.0f, HalfHeight - Radius)};

	const auto StartTop{Start + DistanceToHemisphere * ZAxis};
	const auto StartBottom{Start - DistanceToHemisphere * ZAxis};

	const auto EndTop{End + DistanceToHemisphere * ZAxis};
	const auto EndBottom{End - DistanceToHemisphere * ZAxis};

	DrawHalfCircle(World, StartTop, YAxis, XAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);
	DrawHalfCircle(World, StartTop, YAxis, ZAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawQuarterCircle(World, StartTop, XAxisInverse, ZAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawHalfCircle(World, StartBottom, YAxis, XAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);
	DrawHalfCircle(World, StartBottom, YAxis, ZAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawQuarterCircle(World, StartBottom, XAxisInverse, ZAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawHalfCircle(World, EndTop, YAxis, XAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);
	DrawHalfCircle(World, EndTop, YAxis, ZAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawQuarterCircle(World, EndTop, XAxis, ZAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawHalfCircle(World, EndBottom, YAxis, XAxis, Radius, SweepFColor, Duration, Thickness, DepthPriority);
	DrawHalfCircle(World, EndBottom, YAxis, ZAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawQuarterCircle(World, EndBottom, XAxis, ZAxisInverse, Radius, SweepFColor, Duration, Thickness, DepthPriority);

	DrawDebugLine(World, StartTop - Radius * XAxis, StartBottom - Radius * XAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartTop + Radius * YAxis, StartBottom + Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartTop - Radius * YAxis, StartBottom - Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, EndTop + Radius * XAxis, EndBottom + Radius * XAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, EndTop + Radius * YAxis, EndBottom + Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, EndTop - Radius * YAxis, EndBottom - Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartTop + Radius * ZAxis, EndTop + Radius * ZAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartTop - Radius * YAxis, EndTop - Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartTop + Radius * YAxis, EndTop + Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartBottom - Radius * ZAxis, EndBottom - Radius * ZAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartBottom + Radius * YAxis, EndBottom + Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugLine(World, StartBottom - Radius * YAxis, EndBottom - Radius * YAxis,
		SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	DrawDebugDirectionalArrow(World, Start, End, DrawArrowSize, SweepFColor, bPersistent, Duration, DepthPriority, Thickness);

	if (bHit && Hit.bBlockingHit)
	{
		const auto HitFColor{HitColor.ToFColor(true)};

		DrawDebugCapsule(World, Hit.Location, HalfHeight, Radius, Rotation,
			HitFColor, bPersistent, Duration, DepthPriority, Thickness);

		DrawDebugPoint(World, Hit.ImpactPoint, DrawImpactPointSize, HitFColor, bPersistent, Duration, DepthPriority);
	}
#endif
}


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
