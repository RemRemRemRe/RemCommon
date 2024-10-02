// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "GameplayTag/RemGameplayTagStatics.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
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

uint32 GetHashForTags(const TConstArrayView<FGameplayTag> Tags)
{
	uint32 HashResult{GetTypeHash(Tags.Num())};

	for (auto& Tag : Tags)
	{
		HashResult = HashCombineFast(HashResult, GetTypeHash(Tag));
	}

	return HashResult;
}

uint32 GetHashForTags(const FGameplayTagContainer& Tags)
{
	return GetHashForTags(Tags.GetGameplayTagArray());
}

FString TagToStringWithoutDot(const FGameplayTag& Tag)
{
	return Tag.GetTagName().ToString().Replace(TEXTVIEW(".").GetData(), TEXTVIEW("").GetData());
}

FName TagToNameWithoutDot(const FGameplayTag& Tag)
{
	return FName{TagToStringWithoutDot(Tag)};
}

FGameplayTag TryGetTagFromString(const FString& TagString)
{
	if (auto& Manager = UGameplayTagsManager::Get();
		Manager.IsValidGameplayTagString(TagString))
	{
		const auto Tag = FGameplayTag::RequestGameplayTag(*TagString, false);
		return Tag;
	}
	return FGameplayTag::EmptyTag;
}

bool TryUpdateTagString(FString& StringRef, const FGameplayTag& Tag)
{
	if (StringRef.IsEmpty())
	{
		// empty string, use tag name string by default
		StringRef = Tag.GetTagName().ToString();
	}
	else if (const auto TagFromComment = TryGetTagFromString(StringRef);
		TagFromComment.IsValid() && TagFromComment != Tag)
	{
		// used tag name string but not matched
		StringRef = Tag.GetTagName().ToString();
	}
	else
	{
		// did nothing
		return false;
	}

	// updated
	return true;
}
}
