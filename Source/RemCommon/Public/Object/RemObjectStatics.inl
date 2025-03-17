// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemAssertionMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

class UMovementComponent;

namespace Rem::Object
{

template<typename T = UGameInstance>
requires std::is_base_of_v<UGameInstance, T>
T* GetGameInstance(const UObject& Object)
{
	auto* World = Object.GetWorld();
	RemCheckVariable(World, return nullptr);

	return World->GetGameInstance<T>();
}

template<Concepts::is_player_state T = APlayerState>
T* GetPlayerStateFromPawnOwner(const UActorComponent& Component)
{
	auto* Pawn = Component.GetOwner<APawn>();
	RemEnsureVariable(Pawn, return {});

	return Pawn->GetPlayerState<T>();
}

template<Concepts::is_player_controller T = APlayerController>
T* GetControllerFromPawnOwner(const UActorComponent& Component)
{
	auto* Pawn = Component.GetOwner<APawn>();
	RemEnsureVariable(Pawn, return {});

	return Pawn->GetController<T>();
}

template<Concepts::is_player_state T = APlayerState, bool bRecursive = false>
T* GetPlayerState(const AActor& Actor)
{
	T* Result{nullptr};
	if (const auto* Pawn = Cast<APawn>(&Actor))
	{
		Result = Pawn->GetPlayerState<T>();
	}
	else if (const auto* PlayerController = Cast<APlayerController>(&Actor))
	{
		Result = PlayerController->GetPlayerState<T>();
	}
	else if (const T* PlayerState = Cast<T>(&Actor))
	{
		Result = const_cast<T*>(PlayerState);
	}

	if constexpr (bRecursive)
	{
		if (!Result)
		{
			// try to find player state from owner chain
			if (const auto* Owner = Actor.GetOwner())
			{
				return GetPlayerState<T, bRecursive>(*Owner);
			}
		}
	}

	return Result;
}

template<Concepts::is_player_state T = APlayerState>
T* GetPlayerStateRecursive(const AActor& Actor)
{
	return GetPlayerState<T, true>(Actor);
}

template<Concepts::is_player_controller T = APlayerController>
T* GetFirstLocalPlayerController(const UObject& WorldContextObject)
{
	// ReSharper disable once CommentTypo
	// https://wizardcell.com/unreal/multiplayer-tips-and-tricks/#2-beware-of-getplayerxxx0-static-functions

	const auto* GameInstance = GetGameInstance(WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return Cast<T>(GameInstance->GetFirstLocalPlayerController());
}

template<Concepts::is_hud T = AHUD>
T* GetFirstLocalHUD(const UObject& WorldContextObject)
{
	const auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return PlayerController->GetHUD<T>();
}

template<Concepts::is_local_player T = ULocalPlayer>
T* GetFirstLocalPlayer(const UObject& WorldContextObject)
{
	const auto* GameInstance = GetGameInstance(WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return Cast<T>(GameInstance->GetFirstGamePlayer());
}

template<Concepts::is_pawn T = APawn>
T* GetFirstLocalPlayerPawn(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return PlayerController->GetPawn<T>();
}

template<Concepts::is_player_state T = APlayerState>
T* GetFirstLocalPlayerState(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return PlayerController->GetPlayerState<T>();
}

template<Concepts::is_player_camera_manager T = APlayerCameraManager>
T* GetFirstLocalPlayerCameraManager(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return Cast<T>(PlayerController->PlayerCameraManager);
}

template<Concepts::is_uobject T>
[[nodiscard]] uint32 GetHashForObjects(const TConstArrayView<T*> Objects)
{
	uint32 HashResult{GetTypeHash(Objects.Num())};

	for (auto* Object : Objects)
	{
		HashResult = HashCombineFast(HashResult, GetTypeHash(FWeakObjectPtr{Object}));
	}

	return HashResult;
}

template<typename T = UMovementComponent>
requires std::is_base_of_v<UMovementComponent, T>
T* FindMovementComponent(const AActor& Actor)
{
	if (auto* Pawn = Cast<APawn>(&Actor))
	{
		return Cast<T>(Pawn->GetMovementComponent());
	}

	return Actor.FindComponentByClass<T>();
}

}
