// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/RemObjectStatics.h"

#include "GameFramework/PlayerState.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/RemAssertionMacros.h"

UObject* URemObjectStatics::GetObject(const TSoftObjectPtr<> SoftObjectPtr, UClass* ObjectClass)
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

void URemObjectStatics::ServerViewPreviousPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	RemCheckVariable(PlayerController, return;);

	// player index 0 on Dedicated server is invalid here
	RemCheckCondition(!PlayerController->IsNetMode(NM_DedicatedServer), return;);

	PlayerController->ServerViewPrevPlayer();
}

void URemObjectStatics::ServerViewNextPlayer(const UObject* WorldContextObject)
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	RemCheckVariable(PlayerController, return;);

	// player index 0 on Dedicated server is invalid here
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
