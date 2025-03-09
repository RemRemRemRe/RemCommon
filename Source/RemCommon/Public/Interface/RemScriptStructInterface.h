// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "RemScriptStructInterface.generated.h"

UINTERFACE(MinimalAPI)
class URemScriptStructInterface : public UInterface
{
	GENERATED_BODY()
};

class REMCOMMON_API IRemScriptStructInterface
{
	GENERATED_BODY()

public:
	virtual UScriptStruct& GetScriptStruct() const;
};
