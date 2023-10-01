// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RemObjectStatics.h"

#include "GameFramework/PlayerState.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/RemAssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemObjectStatics)

UObject* URemObjectStatics::GetObject(const TSoftObjectPtr<>& SoftObjectPtr, UClass*)
{
	return SoftObjectPtr.Get();
}

bool URemObjectStatics::IsBlueprintObject(const UObject* Object)
{
	RemCheckVariable(Object, false);
	
	return Object->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint)
		|| !Object->GetClass()->HasAnyClassFlags(CLASS_Native);
}

bool URemObjectStatics::IsObjectValidForBlueprint(const UObject* Object)
{
	RemCheckVariable(Object, false);
	
	return !Object->HasAnyFlags(RF_BeginDestroyed) && !Object->IsUnreachable();
}

APlayerState* URemObjectStatics::GetPlayerState(const AActor* Actor)
{
	RemEnsureVariable(Actor, return {});
	
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		return Pawn->GetPlayerState();
	}
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(Actor))
	{
		return PlayerController->GetPlayerState<APlayerState>();
	}

	if (const APlayerState* PlayerState = Cast<APlayerState>(Actor))
	{
		return const_cast<APlayerState*>(PlayerState);
	}
	
	return {};
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
	// ReSharper disable once CommentTypo
	// https://wizardcell.com/unreal/multiplayer-tips-and-tricks/#2-beware-of-getplayerxxx0-static-functions
	
	const auto* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return GameInstance->GetFirstLocalPlayerController();
}

ULocalPlayer* URemObjectStatics::GetFirstLocalPlayer(const UObject* WorldContextObject)
{
	const auto* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return GameInstance->GetFirstGamePlayer();
}

APawn* URemObjectStatics::GetFirstLocalPlayerPawn(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_AND_ASSERTION);

	return PlayerController->GetPawn();
}

APlayerState* URemObjectStatics::GetFirstLocalPlayerState(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_AND_ASSERTION);

	return PlayerController->GetPlayerState<APlayerState>();
}

APlayerCameraManager* URemObjectStatics::GetFirstLocalPlayerCameraManager(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_AND_ASSERTION);

	return PlayerController->PlayerCameraManager;
}

void URemObjectStatics::ServerViewPreviousPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_AND_ASSERTION);

	// Dedicated server dose not have "local" player controller
	RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewPrevPlayer();
}

void URemObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_AND_ASSERTION);

	// Dedicated server dose not have "local" player controller
    RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);
	
	PlayerController->ServerViewNextPlayer();
}

namespace Rem::Common::Object
{
	
void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
	const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate)
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
	return Function && ensure(Function->GetOuter())
		&& Function->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
}
	
}
