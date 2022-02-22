// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Macro/CoreMiscDefines.h"
#include "CommonLog.h"
#include "StringInterface.generated.h"

/** Interface for printing a object in string format */
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UStringInterface : public UInterface
{
	GENERATED_BODY()
};

class COMMON_API IStringInterface
{
	GENERATED_BODY()

public:
	FString ToString() const;

protected:
	virtual FString ToStringInternal() const
	VIRTUAL_WARN(LogCommon, IStringInterface::ToStringInternal, return {}; );
	
};
