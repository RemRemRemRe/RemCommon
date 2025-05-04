// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Object/RemObjectStatics.h"
#include "Object/RemObjectStatics.inl"

#include "TimerManager.h"
#include "Engine/TimerHandle.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/NavMovementComponent.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Macro/RemAssertionMacros.h"
#include "AudioDeviceHandle.h"
#include "Math/RemMath.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemObjectStatics)

UObject* URemObjectStatics::GetObject(const TSoftObjectPtr<>& SoftObjectPtr, UClass*)
{
	return SoftObjectPtr.Get();
}

bool URemObjectStatics::IsBlueprintObject(const UObject* Object)
{
	RemCheckVariable(Object, return false);

	return Object->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint)
		|| !Object->GetClass()->HasAnyClassFlags(CLASS_Native);
}

UGameInstance* URemObjectStatics::GetGameInstance(const UObject* Object)
{
	RemCheckVariable(Object, return nullptr);

	return Rem::Object::GetGameInstance(*Object);
}

FAudioDeviceHandle URemObjectStatics::GetAudioDevice(const UObject* Object)
{
	RemCheckVariable(Object, return {});

	return Rem::Object::GetAudioDevice(*Object);
}

bool URemObjectStatics::IsObjectValidForBlueprint(const UObject* Object)
{
	RemCheckVariable(Object, return false);

	return !Object->HasAnyFlags(RF_BeginDestroyed) && !Object->IsUnreachable();
}

APlayerState* URemObjectStatics::GetPlayerState(const AActor* Actor)
{
	RemEnsureVariable(Actor, return {});

	return Rem::Object::GetPlayerState<APlayerState>(*Actor);
}

void URemObjectStatics::ShouldNotHappen(const bool bTriggerBreakpointInCpp)
{
	if (bTriggerBreakpointInCpp)
	{
		RemCheckCondition(false);
	}
}

APlayerController* URemObjectStatics::GetFirstLocalPlayerController(const UObject* WorldContextObject)
{
	RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
	return Rem::Object::GetFirstLocalPlayerController(*WorldContextObject);
}

ULocalPlayer* URemObjectStatics::GetFirstLocalPlayer(const UObject* WorldContextObject)
{
	RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
	return Rem::Object::GetFirstLocalPlayer(*WorldContextObject);
}

APawn* URemObjectStatics::GetFirstLocalPlayerPawn(const UObject* WorldContextObject)
{
	RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
	return Rem::Object::GetFirstLocalPlayerPawn(*WorldContextObject);
}

APlayerState* URemObjectStatics::GetFirstLocalPlayerState(const UObject* WorldContextObject)
{
	RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
	return Rem::Object::GetFirstLocalPlayerState(*WorldContextObject);
}

APlayerCameraManager* URemObjectStatics::GetFirstLocalPlayerCameraManager(const UObject* WorldContextObject)
{
	RemCheckVariable(WorldContextObject, return nullptr;, REM_NO_LOG_BUT_ENSURE);
	return Rem::Object::GetFirstLocalPlayerCameraManager(*WorldContextObject);
}

void URemObjectStatics::ServerViewPreviousPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_BUT_ENSURE);

	// Dedicated server dose not have "local" player controller
	RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewPrevPlayer();
}

void URemObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_BUT_ENSURE);

	// Dedicated server dose not have "local" player controller
    RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewNextPlayer();
}

bool URemObjectStatics::SetActorRootComponent(AActor* Actor, USceneComponent* NewRootComponent)
{
	RemCheckVariable(Actor, return false;, REM_NO_LOG_BUT_ENSURE);

	return Actor->SetRootComponent(NewRootComponent);
}

FAudioDeviceHandle Rem::Object::GetAudioDevice(const UObject& Object)
{
	auto* World = Object.GetWorld();
	RemCheckVariable(World, return {});

	return World->GetAudioDevice();
}

namespace Rem::Object
{

void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
	const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)>& Predicate)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner);
	RemCheckVariable(ObjectProperty, return;);

	FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

	for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
	{
		uint8* PropertyValueAddress = Helper.GetRawPtr(DynamicIndex);

		Predicate(PropertyValueAddress, DynamicIndex);
	}
}

bool IsImplementedInBlueprint(const UFunction* Function)
{
	return Function && REM_ENSURE(Function->GetOuter())
		&& Function->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
}

bool CheckPropertyChainByNames(const FEditPropertyChain& PropertyChain, const TArray<FName>& PropertyNamePath, const bool bShouldHaveNextNode)
{
	const auto* MemberNode = PropertyChain.GetActiveMemberNode();
	for (auto& Name : PropertyNamePath)
	{
		if (!MemberNode)
		{
			// different property path
			return false;
		}

		if (const auto* Value = MemberNode->GetValue();
			!Value || Value->GetFName() != Name)
		{
			// different property path
			return false;
		}

		MemberNode = MemberNode->GetNextNode();
	}

	if ((!bShouldHaveNextNode && MemberNode)
		|| (bShouldHaveNextNode && !MemberNode))
	{
		return false;
	}

	return true;
}

FTimerHandle SetTimerForThisTick(const UObject& WorldContextObject, const FTimerDelegate& Delegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	return SetTimerForThisTick(*World, Delegate);
}

FTimerHandle SetTimerForThisTick(const UWorld& World, const FTimerDelegate& Delegate)
{
	return World.GetTimerManager().SetTimerForNextTick([=]
	{
		Delegate.Execute();
	});
}

FTimerHandle SetTimerForNextTick(const UWorld& World, const FTimerDelegate& Delegate)
{
	FTimerHandle Handle;
	World.GetTimerManager().SetTimer(Handle, [=]
	{
		Delegate.Execute();
	}, UE_SMALL_NUMBER, {});

	return Handle;
}

FTimerHandle SetTimerForNextTick(const UObject& WorldContextObject, const FTimerDelegate& Delegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	return SetTimerForNextTick(*World, Delegate);
}

FVector GetActorFeetLocation(const AActor& Actor)
{
	if (const auto* MovementComponent = FindMovementComponent<UNavMovementComponent>(Actor))
	{
		return MovementComponent->GetActorFeetLocation();
	}

	RemCheckCondition(Actor.GetRootComponent(), return FVector::ZeroVector, REM_NO_LOG_BUT_ENSURE);
	return Actor.GetRootComponent()->GetComponentLocation() - FVector{0.0f, 0.0f, Actor.GetRootComponent()->Bounds.BoxExtent.Z};
}

FVector2f GetScreenCenterToMouseVector2F(const APlayerController& PlayerController)
{
	FVector2f MousePosition;
	if (const auto bSuccess = PlayerController.GetMousePosition(MousePosition.X, MousePosition.Y);
		!bSuccess)
	{
		return FVector2f::ZeroVector;
	}
	//RemCheckCondition(bSuccess, return FVector2f::ZeroVector);

	FVector2f ViewportCenter;
	{
		FIntVector2 ViewportSize;
		PlayerController.GetViewportSize(ViewportSize.X, ViewportSize.Y);

		ViewportCenter = FVector2f{static_cast<float>(ViewportSize.X) / 2.0f, static_cast<float>(ViewportSize.Y) / 2.0f};
	}

	// return MousePosition - ViewportCenter;
	return {MousePosition.X - ViewportCenter.X, ViewportCenter.Y - MousePosition.Y};
}

FVector2f GetScreenCenterToMouseDirection2F(const APlayerController& PlayerController)
{
	return GetScreenCenterToMouseVector2F(PlayerController).GetSafeNormal();
}

FVector2f GetScreenCenterToMouseAsWorldDirection2F(const APlayerController& PlayerController)
{
	const auto ForwardDirection{Math::AngleToDirectionXY(PlayerController.GetControlRotation().Yaw)};
	const auto RightDirection{Math::PerpendicularCounterClockwiseXY(ForwardDirection)};

	const auto ScreenSpaceDirection{GetScreenCenterToMouseDirection2F(PlayerController)};
	return FVector2f{ForwardDirection * ScreenSpaceDirection.Y + RightDirection * ScreenSpaceDirection.X}.GetSafeNormal();
}

}
