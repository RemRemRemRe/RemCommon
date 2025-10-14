// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Macro/RemGenerateMembersMacro.h"
#include "RemGameplayTagWithCategory.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemGameplayTagWithCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rem")
	FGameplayTag Tag;

private:
#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Category = "Rem")
	FGameplayTag Category;

#endif

public:
	FRemGameplayTagWithCategory(
		const FGameplayTag& InTag = FGameplayTag::EmptyTag
#if WITH_EDITORONLY_DATA
		, const FGameplayTag& InCategory = FGameplayTag::EmptyTag
#endif
		)
	: Tag(InTag)
#if WITH_EDITORONLY_DATA
	, Category(InCategory)
#endif
	{}

	REM_DEFINE_GETTERS_RETURN_REFERENCE_SIMPLE(Tag)

#if WITH_EDITORONLY_DATA

	REM_DEFINE_GETTERS_RETURN_REFERENCE_SIMPLE(Category)

#endif

	void SetTagAndCategory(const FGameplayTag& InTag);

	/** Used so we can have a TMap of this struct */
	friend uint32 GetTypeHash(const FRemGameplayTagWithCategory& TagWithCategory)
	{
		return GetTypeHash(TagWithCategory.GetTag());
	}

	bool operator==(const FRemGameplayTagWithCategory& Other) const
	{
		return GetTag() == Other.GetTag();
	}

	bool operator==(const FGameplayTag& Other) const
	{
		return GetTag() == Other;
	}

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FGameplayTag()
	{
		return GetTag();
	}

	// ReSharper disable once CppNonExplicitConversionOperator
	operator FGameplayTag() const
	{
		return GetTag();
	}

	FName GetTagName() const
	{
		return GetTag().GetTagName();
	}

	bool IsValid() const
	{
		return GetTag().IsValid();
	}
};
