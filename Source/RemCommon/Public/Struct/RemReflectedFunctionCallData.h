// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemReflectedFunctionData.h"

#include "StructUtils/PropertyBag.h"

#include "RemReflectedFunctionCallData.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemReflectedFunctionCallData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	FRemReflectedFunctionData FunctionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TObjectPtr<UObject> ContextObject;

	UPROPERTY(EditAnywhere, Category = "Rem", meta=(FixedLayout))
	FInstancedPropertyBag Parameters;

	void Execute();

	bool TryFillParameters();
};
