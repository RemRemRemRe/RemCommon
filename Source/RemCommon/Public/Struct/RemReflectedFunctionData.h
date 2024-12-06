// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemReflectedFunctionData.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemReflectedFunctionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TObjectPtr<UClass> FunctionOwnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	FName FunctionName;
};
