// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Interface/RemTagEventSourceInterface.h"

#include "GameplayTagContainer.h"
#include "GameplayTag/RemGameplayTagStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemTagEventSourceInterface)

void IRemTagEventSourceInterface::AddOwnedTags(const TConstArrayView<FGameplayTag> Tags)
{
    AddOwnedTags(Rem::GameplayTag::ToTagContainer(Tags));
}

void IRemTagEventSourceInterface::RemoveOwnedTags(const TConstArrayView<FGameplayTag> Tags)
{
    RemoveOwnedTags(Rem::GameplayTag::ToTagContainer(Tags));
}
