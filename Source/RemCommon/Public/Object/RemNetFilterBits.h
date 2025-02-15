// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Engine/EngineBaseTypes.h"

#include "RemNetFilterBits.generated.h"

//enum ENetMode;
enum ENetRole : int;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemNetFilterBits
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Role")
	uint8 bRunOnLocalPlayer : 1 {true};

	UPROPERTY(EditAnywhere, Category = "Role")
	uint8 bRunOnSimulatedProxy : 1 {true};

	UPROPERTY(EditAnywhere, Category = "Net Mode")
	uint8 bRunOnStandalone : 1 {true};

	UPROPERTY(EditAnywhere, Category = "Net Mode")
	uint8 bRunOnDedicatedServer : 1 {true};

	UPROPERTY(EditAnywhere, Category = "Net Mode")
	uint8 bRunOnListenServer : 1 {true};

	bool Matches(ENetMode NetMode, ENetRole Role) const;
};
