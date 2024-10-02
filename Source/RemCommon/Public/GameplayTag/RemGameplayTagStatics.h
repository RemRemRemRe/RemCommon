// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemGameplayTagStatics.generated.h"

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
}
