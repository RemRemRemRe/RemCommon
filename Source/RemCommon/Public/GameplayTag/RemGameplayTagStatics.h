// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemGameplayTagStatics.generated.h"

struct FGameplayTagQuery;
struct FGameplayTagContainer;
struct FGameplayTag;

UCLASS()
class REMCOMMON_API URemGameplayTagStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

};

namespace Rem::GameplayTag
{
	/**
	 * @brief Return the first parent of a given tag. eg: "X.Y.Z" will return "X"
	 * @return first parent of the given tag
	 */
	REMCOMMON_API FGameplayTag GetFirstParent(const FGameplayTag& Tag);

	/**
	 * @brief Using a FGameplayTag to construct a FPrimaryAssetId
	 * @param Tag an identifier for something
	 * @return Say, Tag = "X.A.B", This would return FPrimaryAssetId::PrimaryAssetType = "X", FPrimaryAssetId::PrimaryAssetName = "X.A.B"
	 */
	REMCOMMON_API FPrimaryAssetId MakePrimaryAssetIdFromTag(const FGameplayTag& Tag);

	[[nodiscard]] REMCOMMON_API uint32 GetHashForTags(const TConstArrayView<FGameplayTag> Tags);
	[[nodiscard]] REMCOMMON_API uint32 GetHashForTags(const FGameplayTagContainer& Tags);

	[[nodiscard]] REMCOMMON_API FString TagToStringWithoutDot(const FGameplayTag& Tag);
	[[nodiscard]] REMCOMMON_API FName TagToNameWithoutDot(const FGameplayTag& Tag);

	[[nodiscard]] REMCOMMON_API FGameplayTag TryGetTagFromString(const FString& TagString);

	/**
	 * @param StringRef string to modify
	 * @param Tag string source
	 * @return true if StringRef get modified
	 */
	REMCOMMON_API bool TryUpdateTagString(FString& StringRef, const FGameplayTag& Tag);

	[[nodiscard]] REMCOMMON_API TArray<FString> GetTagsString(TConstArrayView<FGameplayTag> Tags);
	[[nodiscard]] REMCOMMON_API FGameplayTagContainer ToTagContainer(TConstArrayView<FGameplayTag> Tags);
	[[nodiscard]] REMCOMMON_API FString ToString(TConstArrayView<FGameplayTag> Tags, bool bQuoted = false);

	REMCOMMON_API FGameplayTag FindCommonParentTag(const FGameplayTag& TagOne, const FGameplayTag& TagTwo);
	REMCOMMON_API FGameplayTag FindCommonParentTag(FStringView TagStringOne, FStringView TagStringTwo);
	[[nodiscard]] REMCOMMON_API TArray<FGameplayTag> FindCommonParentTags(TConstArrayView<FGameplayTag> TagsOne, TConstArrayView<FGameplayTag> TagsTwo);

	REMCOMMON_API bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, TConstArrayView<FGameplayTag> TagsToMatch);
	REMCOMMON_API bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const FGameplayTag& TagToMatch);

	/**
	 * passing FGameplayTagContainer when possible as this overload won't do allocating or anything else
	 */
	REMCOMMON_API bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const FGameplayTagContainer& TagsToMatch);

	REMCOMMON_API bool IsTagQueryMatches(const FGameplayTagQuery& TagQuery, const FGameplayTagContainer& TagsToMatch);
	[[nodiscard]] REMCOMMON_API const FGameplayTagContainer& GetSingleTagContainer(const FGameplayTag& Tag);
}
