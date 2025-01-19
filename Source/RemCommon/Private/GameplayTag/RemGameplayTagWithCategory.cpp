// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "GameplayTag/RemGameplayTagWithCategory.h"

#include "GameplayTag/RemGameplayTagStatics.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(RemGameplayTagWithCategory)

void FRemGameplayTagWithCategory::SetTagAndCategory(const FGameplayTag& InTag)
{
	Tag = InTag;

#if WITH_EDITORONLY_DATA
	
	Category = Rem::GameplayTag::GetFirstParent(GetTag());

#endif
}

