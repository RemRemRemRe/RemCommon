// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Macro/RemAssertionMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

namespace Rem::Object
{

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

template<Concepts::is_player_state T = APlayerState>
T* GetPlayerState(const AActor& Actor)
{
	if (const auto* Pawn = Cast<APawn>(&Actor))
	{
		return Pawn->GetPlayerState<T>();
	}
	
	if (const auto* PlayerController = Cast<APlayerController>(&Actor))
	{
		return PlayerController->GetPlayerState<T>();
	}

	if (const T* PlayerState = Cast<T>(&Actor))
	{
		return const_cast<T*>(PlayerState);
	}
	
	return {};
}

template<Concepts::is_player_controller T = APlayerController>
T* GetFirstLocalPlayerController(const UObject& WorldContextObject)
{
	// ReSharper disable once CommentTypo
	// https://wizardcell.com/unreal/multiplayer-tips-and-tricks/#2-beware-of-getplayerxxx0-static-functions
	
	const auto* GameInstance = UGameplayStatics::GetGameInstance(&WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return Cast<T>(GameInstance->GetFirstLocalPlayerController());
}

template<Concepts::is_local_player T = ULocalPlayer>
T* GetFirstLocalPlayer(const UObject& WorldContextObject)
{
	const auto* GameInstance = UGameplayStatics::GetGameInstance(&WorldContextObject);
	RemCheckVariable(GameInstance, return nullptr;, REM_NO_LOG_BUT_ENSURE);

	return Cast<T>(GameInstance->GetFirstGamePlayer());
}

template<Concepts::is_pawn T = APawn>
T* GetFirstLocalPlayerPawn(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_OR_ASSERTION);

	return PlayerController->GetPawn<T>();
}

template<Concepts::is_player_state T = APlayerState>
T* GetFirstLocalPlayerState(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_OR_ASSERTION);

	return PlayerController->GetPlayerState<T>();
}

template<Concepts::is_player_camera_manager T = APlayerCameraManager>
T* GetFirstLocalPlayerCameraManager(const UObject& WorldContextObject)
{
	auto* PlayerController = GetFirstLocalPlayerController(WorldContextObject);
	RemCheckVariable(PlayerController, return nullptr;, REM_NO_LOG_OR_ASSERTION);

	return Cast<T>(PlayerController->PlayerCameraManager);
}

}
