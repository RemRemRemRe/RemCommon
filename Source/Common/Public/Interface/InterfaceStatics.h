// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "InterfaceStatics.generated.h"

/**
 * 
 */
UCLASS()
class COMMON_API UInterfaceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Utilities|Interface")
	static bool IsValidInterface(const FScriptInterface Interface);
};
