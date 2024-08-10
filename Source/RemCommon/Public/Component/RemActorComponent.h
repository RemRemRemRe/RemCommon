// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"

#include "RemActorComponent.generated.h"


UCLASS()
class REMCOMMON_API URemActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URemActorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
