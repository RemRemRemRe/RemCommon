// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Macro/RemCoreMiscDefines.h"
#include "RemCommonLog.h"
#include "RemStringInterface.generated.h"

/** Interface for printing a object in string format */
UINTERFACE(BlueprintType, NotBlueprintable)
class UStringInterface : public UInterface
{
	GENERATED_BODY()
};

class REMCOMMON_API IStringInterface
{
	GENERATED_BODY()

public:
	FString ToString() const;

protected:
	virtual FString ToStringInternal() const
		REM_VIRTUAL_WARN(LogRemCommon, return {};);
	
};
