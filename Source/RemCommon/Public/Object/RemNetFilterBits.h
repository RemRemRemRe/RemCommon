// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemNetFilterBits.generated.h"

USTRUCT(BlueprintType)
struct FRemNetFilterBits
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

	REMCOMMON_API bool Matches(ENetMode NetMode, ENetRole Role) const;
};