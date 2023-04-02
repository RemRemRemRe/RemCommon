// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectStatics.h"

#include "GameFramework/PlayerState.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/AssertionMacros.h"

UObject* UObjectStatics::GetObject(const TSoftObjectPtr<> SoftObjectPtr, UClass* ObjectClass)
{
	return SoftObjectPtr.Get();
}

bool UObjectStatics::IsBlueprintObject(const UObject* Object)
{
	CheckPointer(Object, false);
	
	return Object->GetClass()->HasAnyClassFlags(CLASS_CompiledFromBlueprint)
		|| !Object->GetClass()->HasAnyClassFlags(CLASS_Native);
}

bool UObjectStatics::IsObjectValidForBlueprint(const UObject* Object)
{
	CheckPointer(Object, false);
	
	return !Object->HasAnyFlags(RF_BeginDestroyed) && !Object->IsUnreachable();
}

APlayerState* UObjectStatics::GetPlayerState(const AActor* Actor)
{
	EnsurePointer(Actor, return {});
	
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

void UObjectStatics::ShouldNotHappen(const bool bTriggerBreakpointInCpp)
{
	if (bTriggerBreakpointInCpp)
	{
		CheckCondition(false);
	}
}

void UObjectStatics::ServerViewPreviousPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	CheckPointer(PlayerController, return;);

	// player index 0 on Dedicated server is invalid here
	CheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewPrevPlayer();
}

void UObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	CheckPointer(PlayerController, return;);

	// player index 0 on Dedicated server is invalid here
    CheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);
	
	PlayerController->ServerViewNextPlayer();
}

namespace Common::ObjectStatics
{
	
void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
	const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner);
	CheckPointer(ObjectProperty, return;);
	
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
