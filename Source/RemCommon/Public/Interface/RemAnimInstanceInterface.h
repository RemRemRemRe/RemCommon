// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "RemAnimInstanceInterface.generated.h"

class UAnimInstance;

UINTERFACE(MinimalAPI)
class URemAnimInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

class REMCOMMON_API IRemAnimInstanceInterface
{
	GENERATED_BODY()

public:
	virtual UAnimInstance* GetAnimInstance(const AActor& Actor) const;
};
