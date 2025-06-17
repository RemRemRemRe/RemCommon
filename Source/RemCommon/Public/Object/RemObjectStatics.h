﻿// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RemObjectStatics.generated.h"

class FAudioDeviceHandle;
class UGameInstance;
class APlayerCameraManager;
class APlayerState;
class APlayerController;
class ULocalPlayer;
struct FTimerManagerTimerParameters;

namespace Rem
{
using FTimerDelegate = TDelegate<void()>;
}

struct FTimerHandle;

#define REM_API REMCOMMON_API

UCLASS()
class REM_API URemObjectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Rem|Object", meta = (DeterminesOutputType = ObjectClass))
	static UObject* GetObject(const TSoftObjectPtr<UObject>& SoftObjectPtr, UClass* ObjectClass);

	/**
	 * @brief Is Object a blueprint object
	 */
	UFUNCTION(BlueprintPure, Category = "Rem|Object", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsBlueprintObject(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Rem|Object")
	static UGameInstance* GetGameInstance(const UObject* Object);

	static FAudioDeviceHandle GetAudioDevice(const UObject* Object);

	/**
	 * @brief Dose the class of the object is blueprint class
	 */
	UFUNCTION(BlueprintPure, Category = "Rem|Object", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool IsObjectValidForBlueprint(const UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Rem|PlayerState")
	static APlayerState* GetPlayerState(const AActor* Actor);

	/**
	 * @brief Being explicit about nothing
	 * @see https://landelare.github.io/2022/04/30/uses-of-a-useless-node.html
	 */
	UFUNCTION(BlueprintCallable, Category = "Rem", Meta = (DevelopmentOnly, CompactNodeTitle = "Do Nothing"))
	static void DoNothing()	{}

	/**
	 * @brief Being explicit about something should not happen
	 */
	UFUNCTION(BlueprintCallable, Category = "Rem", Meta = (DevelopmentOnly, CompactNodeTitle = "Should Not Happen"))
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

	UFUNCTION(BlueprintCallable, Category = "Rem|Actor", meta = (DefaultToSelf = "Actor", ExpandBoolAsExecs = "ReturnValue"))
	static bool SetActorRootComponent(AActor* Actor, USceneComponent* NewRootComponent);
};

namespace Rem::Object
{
	REM_API FAudioDeviceHandle GetAudioDevice(const UObject& Object);

	REM_API void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
		const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)>& Predicate);

	REM_API bool IsImplementedInBlueprint(const UFunction* Function);

	inline bool IsClassDefaultObject(const UObject& Object)
	{
		return static_cast<bool>(Object.GetFlags() & EObjectFlags::RF_ClassDefaultObject);
	}

	constexpr bool IsArrayItselfChanged(const EPropertyChangeType::Type ChangeType)
	{
		return ChangeType & EPropertyChangeType::ArrayAdd
			|| ChangeType & EPropertyChangeType::ArrayRemove
			|| ChangeType & EPropertyChangeType::ArrayClear
			|| ChangeType & EPropertyChangeType::Duplicate
			|| ChangeType & EPropertyChangeType::ArrayMove;
	}

	constexpr bool IsOnlyArrayElementChanged(const EPropertyChangeType::Type ChangeType)
	{
		return ChangeType & EPropertyChangeType::ValueSet
			|| ChangeType & EPropertyChangeType::Redirected;
	}

	REM_API bool CheckPropertyChainByNames(const FEditPropertyChain& PropertyChain, const TArray<FName>& PropertyNamePath, bool bShouldHaveNextNode = false);

	///
	///
	//// Rem::Latent::SetTimerForThisTick is preferred for gameplay as it's more "tick order / dependency" friendly
	///
	///

	REM_API FTimerHandle SetTimerForThisTick(const UWorld& World, const FTimerDelegate& Delegate);
	REM_API FTimerHandle SetTimerForThisTick(const UObject& WorldContextObject, const FTimerDelegate& Delegate);

	REM_API FTimerHandle SetTimerForNextTick(const UWorld& World, const FTimerDelegate& Delegate);
	REM_API FTimerHandle SetTimerForNextTick(const UObject& WorldContextObject, const FTimerDelegate& Delegate);

	REM_API FVector GetActorFeetLocation(const AActor& Actor);

	REM_API FVector2f GetScreenCenterToMouseVector2F(const APlayerController& PlayerController);
	REM_API FVector2f GetScreenCenterToMouseDirection2F(const APlayerController& PlayerController);
	REM_API FVector2f GetScreenCenterToMouseAsWorldDirection2F(const APlayerController& PlayerController);
}

#undef REM_API
