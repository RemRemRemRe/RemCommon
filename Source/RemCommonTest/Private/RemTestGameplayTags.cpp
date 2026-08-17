// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemTestGameplayTags.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

namespace Rem::Test
{
FGameplayTag AddNativeTag(const FName TagName)
{
    auto& TagsManager = UGameplayTagsManager::Get();

    if (const auto ExistingTag = TagsManager.RequestGameplayTag(TagName, false);
        ExistingTag.IsValid())
    {
        return ExistingTag;
    }

    return TagsManager.AddNativeGameplayTag(TagName);
}
}
