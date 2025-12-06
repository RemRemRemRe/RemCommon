// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemDrawDebugStatics.generated.h"

struct FTraceDatum;
struct FHitResult;
namespace EDrawDebugTrace
{
    enum Type : int;
}

#define REM_API REMCOMMON_API

UCLASS()
class REM_API URemDrawDebugStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Rem|Debug Utility",
		Meta = (DefaultToSelf = "Actor", AutoCreateRefTerm = "DisplayName", ReturnDisplayName = "Value"))
	static bool ShouldDisplayDebugForActor(const AActor* Actor, const FName& DisplayName);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Location, XAxis, YAxis, Color"))
	static void DrawHalfCircle(const UObject* WorldContext, const FVector& Location,
		const FVector& XAxis, const FVector& YAxis, float Radius, const FLinearColor& Color,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Location, XAxis, YAxis, Color"))
	static void DrawQuarterCircle(const UObject* WorldContext, const FVector& Location,
		const FVector& XAxis, const FVector& YAxis, float Radius, const FLinearColor& Color,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Location, Rotation, Color"))
	static void DrawSphereAlternative(const UObject* WorldContext, const FVector& Location, const FRotator& Rotation,
		float Radius, const FLinearColor& Color, float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Start, End, Color"))
	static void DrawSweepSphere(const UObject* WorldContext, const FVector& Start, const FVector& End, float Radius,
		const FLinearColor& Color, float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Start, End, TraceColor, HitColor"))
	static void DrawLineTraceSingle(const UObject* WorldContext, const FVector& Start, const FVector& End, bool bHit,
		const FHitResult& Hit, const FLinearColor& TraceColor, const FLinearColor& HitColor,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Start, End, SweepColor, HitColor"))
	static void DrawSweepSingleSphere(const UObject* WorldContext, const FVector& Start, const FVector& End,
		float Radius, bool bHit, const FHitResult& Hit, const FLinearColor& SweepColor, const FLinearColor& HitColor,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
	Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Start, End, Rotation, SweepColor, HitColor"))
	static void DrawSweepSingleCapsule(const UObject* WorldContext, const FVector& Start, const FVector& End,
		const FRotator& Rotation, float Radius, float HalfHeight, bool bHit,
		const FHitResult& Hit, const FLinearColor& SweepColor, const FLinearColor& HitColor,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);

	UFUNCTION(BlueprintCallable, Category = "Rem|Debug Utility",
		Meta = (WorldContext = "WorldContext", DevelopmentOnly, AutoCreateRefTerm = "Start, End, SweepColor, HitColor"))
	static void DrawSweepSingleCapsuleAlternative(const UObject* WorldContext, const FVector& Start, const FVector& End,
		float Radius, float HalfHeight, bool bHit, const FHitResult& Hit,
		const FLinearColor& SweepColor, const FLinearColor& HitColor,
		float Duration = 0.0f, float Thickness = 1.0f, uint8 DepthPriority = 0);
};

namespace Rem::DrawDebug
{
	constexpr auto DrawImpactPointSize{32.0f};
	constexpr auto DrawLineThickness{1.0f};
	constexpr auto DrawArrowSize{50.0f};
	constexpr auto DrawCircleSidesCount{16};

	/**
	 * Draw a crosshair on the screen. By default, it draws the crosshair at center of the screen
	 */
	REM_API void DrawDebugCrossHair(const UWorld& World, float CrossHairLineLength = 20.0f,
			float LineThickness = 2.f, float AngleToRotate = 0.0f, const FVector2f& CrossHairCenterScreenSpace = {-1.0f, -1.0f},
		const FLinearColor& LineColor = FLinearColor::White);
    
    REM_API void DrawDebugTraceData(const UWorld& World, const FTraceDatum& TraceDatum, EDrawDebugTrace::Type DrawDebugType,
        const FLinearColor& TraceColor = FLinearColor::Green, const FLinearColor& TraceHitColor = FLinearColor::Red, float DrawTime = -1.0f);
    
}

#undef REM_API
