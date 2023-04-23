// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Macro/RemAssertionMacros.h"
#include "RemObjectStatics.generated.h"

/**
 * 
 */
UCLASS()
class REMCOMMON_API URemObjectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = ObjectClass))
	static UObject* GetObject(TSoftObjectPtr<UObject> SoftObjectPtr, UClass* ObjectClass);
	
	/**
	 * @brief Is Object a blueprint object
	 */
	UFUNCTION(BlueprintPure, Category = "Rem|Object")
	static bool IsBlueprintObject(const UObject* Object);

	/**
	 * @brief Dose the class of the object is blueprint class
	 */
	UFUNCTION(BlueprintPure, Category = "Rem|Object")
	static bool IsObjectValidForBlueprint(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Rem|PlayerState")
	static APlayerState* GetPlayerState(const AActor* Actor);

	/**
	 * @brief Being explicit about nothing
	 * @see https://landelare.github.io/2022/04/30/uses-of-a-useless-node.html
	 */
	UFUNCTION(BlueprintCallable, Meta = (DevelopmentOnly, CompactNodeTitle = "Do Nothing"))
	static void DoNothing()	{}

	/**
	 * @brief Being explicit about something should not happen
	 */
	UFUNCTION(BlueprintCallable, Meta = (DevelopmentOnly, CompactNodeTitle = "Should Not Happen"))
	static void ShouldNotHappen(const bool bTriggerBreakpointInCpp = true);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static void ServerViewPreviousPlayer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static void ServerViewNextPlayer(const UObject* WorldContextObject);
};

namespace Rem::Common::Object
{
	REMCOMMON_API void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
		TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate);

	REMCOMMON_API bool IsImplementedInBlueprint(const UFunction* Function);

	/** Templated version of FindComponentByClass that handles casting for you */
	template<class T>
	requires std::is_base_of_v<UActorComponent, T>
	T* FindComponentByClass(const AActor* Actor)
	{
		RemEnsureVariable(Actor, return {});
		return Actor->FindComponentByClass<T>();
	}

	template<typename T = APlayerState>
	requires std::is_base_of_v<APlayerState, T>
	T* GetPlayerStateFromPawnOwner(const UActorComponent* Component)
	{
		RemEnsureVariable(Component, return {});

		APawn* Pawn = Component->GetOwner<APawn>();
		
		RemEnsureVariable(Pawn, return {});

		return Pawn->GetPlayerState<T>();
	}

	template<typename T = APlayerController>
	requires std::is_base_of_v<APlayerController, T>
	T* GetControllerFromPawnOwner(const UActorComponent* Component)
	{
		RemEnsureVariable(Component, return {});

		APawn* Pawn = Component->GetOwner<APawn>();
		
		RemEnsureVariable(Pawn, return {});

		return Pawn->GetController<T>();
	}
	
}
