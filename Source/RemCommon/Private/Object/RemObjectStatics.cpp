// Copyright RemRemRemRe, All Rights Reserved.


#include "Object/RemObjectStatics.h"
#include "Object/RemObjectStatics.inl"

#include "TimerManager.h"
#include "Engine/TimerHandle.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Macro/RemAssertionMacros.h"

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
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_OR_ASSERTION);

	// Dedicated server dose not have "local" player controller
	RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewPrevPlayer();
}

void URemObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return;, REM_NO_LOG_OR_ASSERTION);

	// Dedicated server dose not have "local" player controller
    RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewNextPlayer();
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
	return Function && ensure(Function->GetOuter())
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

FTimerHandle SetTimerForThisTick(const UObject& WorldContextObject, const FTimerDelegate& InDelegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	return SetTimerForThisTick(*World, InDelegate);
}

FTimerHandle SetTimerForThisTick(const UWorld& World, const FTimerDelegate& InDelegate)
{
	return World.GetTimerManager().SetTimerForNextTick(InDelegate);
}

FTimerHandle SetTimerForNextTick(const UWorld& World, const FTimerDelegate& InDelegate)
{
	FTimerHandle Handle;
	World.GetTimerManager().SetTimer(Handle, InDelegate, UE_SMALL_NUMBER, {});

	return Handle;
}

FTimerHandle SetTimerForNextTick(const UObject& WorldContextObject, const FTimerDelegate& InDelegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	return SetTimerForNextTick(*World, InDelegate);
}

}
