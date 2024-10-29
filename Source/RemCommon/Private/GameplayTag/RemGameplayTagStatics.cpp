// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "GameplayTag/RemGameplayTagStatics.h"

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Macro/RemAssertionMacros.h"
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

TArray<FString> GetTagsString(const TConstArrayView<FGameplayTag> Tags)
{
	TArray<FString> TagsString{};
	TagsString.Reserve(Tags.Num());

	for (auto& TagOne : Tags)
	{
		TagsString.Add(TagOne.ToString());
	}

	return TagsString;
}

FGameplayTagContainer ToTagContainer(const TConstArrayView<FGameplayTag> Tags)
{
	return FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>(Tags));
}

FString ToString(const TConstArrayView<FGameplayTag> Tags, const bool bQuoted)
{
	FString RetString;
	for (int32 Index = 0; Index < Tags.Num(); ++Index)
	{
		if (bQuoted)
		{
			RetString += TEXT("\"");
		}
		RetString += Tags[Index].ToString();
		if (bQuoted)
		{
			RetString += TEXT("\"");
		}

		if (Index < Tags.Num() - 1)
		{
			RetString += TEXT(", ");
		}
	}
	return RetString;
}

FGameplayTag FindCommonParentTag(const FGameplayTag& TagOne, const FGameplayTag& TagTwo)
{
	RemCheckVariable(TagOne, return {});
	RemCheckVariable(TagTwo, return {});

	if (TagOne == TagTwo)
	{
		return TagOne;
	}

	// could we get string view of tag?
	const auto StringOne = TagOne.ToString();
	const auto StringTwo = TagTwo.ToString();

	return FindCommonParentTag(StringOne, StringTwo);
}

FGameplayTag FindCommonParentTag(const FStringView TagStringOne, const FStringView TagStringTwo)
{
	const auto MinLength = FMath::Min(TagStringOne.Len(), TagStringTwo.Len());
	int32 DotIndex{INDEX_NONE};

	for (int32 Index= 0; Index < MinLength; ++Index)
	{
		if (TagStringOne[Index] != TagStringTwo[Index])
		{
			break;
		}

		if (TagStringOne[Index] == '.')
		{
			DotIndex = Index;
		}
	}

	if (DotIndex == INDEX_NONE)
	{
		return {};
	}

	return FGameplayTag::RequestGameplayTag(*FString{TagStringOne.SubStr(0, DotIndex)});
}

TArray<FGameplayTag> FindCommonParentTags(const TConstArrayView<FGameplayTag> TagsOne, const TConstArrayView<FGameplayTag> TagsTwo)
{
	const auto Number = FMath::Min(TagsOne.Num(), TagsTwo.Num());
	if (Number == 0)
	{
		return {};
	}

	TArray<FGameplayTag> Results{};
	Results.Reserve(Number);

	const auto TagsStringOne = GetTagsString(TagsOne);
	const auto TagsStringTwo = GetTagsString(TagsTwo);

	for (auto& TagOne : TagsStringOne)
	{
		for (auto& TagTwo : TagsStringTwo)
		{
			if (auto CommonTag = FindCommonParentTag(TagOne, TagTwo);
				CommonTag.IsValid())
			{
				Results.Add(CommonTag);
			}
		}
	}

	return Results;
}

bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const TConstArrayView<FGameplayTag> TagsToMatch)
{
	return IsTagQueryMatches(TagQuery, ToTagContainer(TagsToMatch));
}

bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const FGameplayTag& TagToMatch)
{
	return IsTagQueryMatches(TagQuery, TagToMatch.GetSingleTagContainer());
}

bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const FGameplayTagContainer& TagsToMatch)
{
	if (TagQuery.IsEmpty())
	{
		return true;
	}

	return TagQuery.Matches(TagsToMatch);
}

}
