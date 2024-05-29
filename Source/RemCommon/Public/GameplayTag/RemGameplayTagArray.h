// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "RemGameplayTagArray.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemGameplayTagArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditInline, TitleProperty = TagName))
	TArray<FGameplayTag> Tags;
};
