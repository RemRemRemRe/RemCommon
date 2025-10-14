// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Macro/RemGenerateMembersMacro.h"

#include "RemGameplayTagArray.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemGameplayTagArray
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Category = "Rem")
    FGameplayTag OptionalCategory;
    
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (EditInline, TitleProperty = TagName))
	TArray<FGameplayTag> Tags;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Tags)
};
