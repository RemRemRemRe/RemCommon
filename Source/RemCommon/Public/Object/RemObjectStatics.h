﻿// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemObjectStatics.generated.h"


UCLASS()
class REMCOMMON_API URemObjectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = ObjectClass))
	static UObject* GetObject(const TSoftObjectPtr<UObject>& SoftObjectPtr, UClass* ObjectClass);

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
	static APlayerController* GetFirstLocalPlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static ULocalPlayer* GetFirstLocalPlayer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static APawn* GetFirstLocalPlayerPawn(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static APlayerState* GetFirstLocalPlayerState(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static APlayerCameraManager* GetFirstLocalPlayerCameraManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static void ServerViewPreviousPlayer(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Rem|PlayerController", meta = (WorldContext = "WorldContextObject"))
	static void ServerViewNextPlayer(const UObject* WorldContextObject);
};

namespace Rem::Object
{
	REMCOMMON_API void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
		const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)>& Predicate);

	REMCOMMON_API bool IsImplementedInBlueprint(const UFunction* Function);

	inline bool IsClassDefaultObject(const UObject& Object)
	{
		return static_cast<bool>(Object.GetFlags() & EObjectFlags::RF_ClassDefaultObject);
	}

	constexpr bool IsArrayItselfChanged(const EPropertyChangeType::Type ChangeType)
	{
		return ChangeType & EPropertyChangeType::ArrayAdd
			|| ChangeType & EPropertyChangeType::ArrayRemove
			|| ChangeType & EPropertyChangeType::Duplicate
			|| ChangeType & EPropertyChangeType::ArrayMove;
	}

	constexpr bool IsOnlyArrayElementChanged(const EPropertyChangeType::Type ChangeType)
	{
		return ChangeType & EPropertyChangeType::ValueSet
			|| ChangeType & EPropertyChangeType::Redirected;
	}

	REMCOMMON_API bool CheckPropertyChainByNames(const FEditPropertyChain& PropertyChain, const TArray<FName>& PropertyNamePath, bool bShouldHaveNextNode = false);
}
