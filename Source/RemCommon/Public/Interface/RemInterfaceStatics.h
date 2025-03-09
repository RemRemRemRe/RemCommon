﻿// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemInterfaceStatics.generated.h"


UCLASS()
class REMCOMMON_API URemInterfaceStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Rem|Utilities|Interface", meta = (AutoCreateRefTerm = Interface))
	static bool IsValidInterface(const FScriptInterface& Interface);
};
