// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Macro/AssertionMacros.h"
#include "ObjectStatics.generated.h"

/**
 * 
 */
UCLASS()
class COMMON_API UObjectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = ObjectClass))
	static UObject* GetObject(TSoftObjectPtr<UObject> SoftObjectPtr, UClass* ObjectClass);
	
	/**
	 * @brief Is Object a blueprint object
	 */
	UFUNCTION(BlueprintPure, Category = "Object")
	static bool IsBlueprintObject(const UObject* Object);

	/**
	 * @brief Dose the class of the object is blueprint class
	 */
	UFUNCTION(BlueprintPure, Category = "Object")
	static bool IsObjectValidForBlueprint(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "PlayerState")
	static APlayerState* GetPlayerState(const AActor* Actor);
	
};

namespace Common::ObjectStatics
{
	COMMON_API void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
		TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate);

	COMMON_API bool IsImplementedInBlueprint(const UFunction* Function);

	/** Templated version of FindComponentByClass that handles casting for you */
	template<class T>
	static T* FindComponentByClass(const AActor* Actor)
	{
		static_assert(TIsDerivedFrom<T, UActorComponent>::Value, "T must be an UActorComponent");
		
		EnsurePointer(Actor, return {});
		return Actor->FindComponentByClass<T>();
	}

	template<typename T = APlayerState>
	T* GetPlayerStateFromPawnOwner(const UActorComponent* Component)
	{
		static_assert(TIsDerivedFrom<T, APlayerState>::Value, "T must be an APlayerState");

		EnsurePointer(Component, return {});

		APawn* Pawn = Component->GetOwner<APawn>();
		
		EnsurePointer(Pawn, return {});

		return Pawn->GetPlayerState<T>();
	}

	template<typename T = APlayerController>
	T* GetControllerFromPawnOwner(const UActorComponent* Component)
	{
		static_assert(TIsDerivedFrom<T, APlayerController>::Value, "T must be an APlayerController");

		EnsurePointer(Component, return {});

		APawn* Pawn = Component->GetOwner<APawn>();
		
		EnsurePointer(Pawn, return {});

		return Pawn->GetController<T>();
	}

	
}
