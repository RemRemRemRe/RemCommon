﻿// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "GameplayTag/RemGameplayTagStatics.h"

#include "GameplayTagContainer.h"
#include "UObject/PrimaryAssetId.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemGameplayTagStatics)

namespace Rem::GameplayTag
{

FGameplayTag GetFirstParent(const FGameplayTag& Tag)
{
	return Tag.GetGameplayTagParents().Last();
}

FPrimaryAssetId MakePrimaryAssetIdFromTag(const FGameplayTag& Tag)
{
	return FPrimaryAssetId{GetFirstParent(Tag).GetTagName(), Tag.GetTagName()};
}

}
