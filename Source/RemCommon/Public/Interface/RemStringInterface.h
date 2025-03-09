// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "Macro/RemCoreMiscDefines.h"
#include "RemCommonLog.h"

#include "RemStringInterface.generated.h"

UINTERFACE(MinimalAPI)
class URemStringInterface : public UInterface
{
	GENERATED_BODY()
};

/** Interface for printing a object in string format */
class REMCOMMON_API IRemStringInterface
{
	GENERATED_BODY()

public:
	virtual FString ToString() const
		REM_VIRTUAL_WARN(LogRemCommon, return {};);

};
