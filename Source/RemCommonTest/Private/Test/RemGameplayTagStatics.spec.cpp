// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "GameplayTagsManager.h"
#include "GameplayTag/RemGameplayTagStatics.h"
#include "GameplayTag/RemGameplayTagWithCategory.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::GameplayTag::GetFirstParent;
using Rem::GameplayTag::MakePrimaryAssetIdFromTag;
using Rem::GameplayTag::TagToStringWithoutDot;
using Rem::GameplayTag::TagToNameWithoutDot;
using Rem::GameplayTag::TryGetTagFromString;
using Rem::GameplayTag::TryUpdateTagString;
using Rem::GameplayTag::FindCommonParentTag;
using Rem::GameplayTag::IsTagQueryMatches;
using Rem::GameplayTag::GetHashForTags;
using Rem::GameplayTag::ToTagContainer;
using Rem::GameplayTag::GetSingleTagContainer;

FGameplayTag AddTag(const FStringView TagName)
{
    auto& TagsManager = UGameplayTagsManager::Get();
    if (const auto ExistingTag = TagsManager.RequestGameplayTag(FName{TagName}, false);
        ExistingTag.IsValid())
    {
        return ExistingTag;
    }
    return TagsManager.AddNativeGameplayTag(FName{TagName});
}
}

DEFINE_SPEC(FRemGameplayTagStaticsTest, "Rem.Common.GameplayTag",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemGameplayTagStaticsTest::Define()
{
    Describe(TEXT("Parent and asset id"), [this]
    {
        It(TEXT("GetFirstParent should return the root-most parent"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Leaf"));

            TestEqual(TEXT("root parent"), GetFirstParent(Tag), AddTag(TEXT("Test")));
        });

        It(TEXT("GetFirstParent should return the tag itself for a root tag"), [this]
        {
            const auto Tag = AddTag(TEXT("TestCommonRoot"));

            TestEqual(TEXT("self"), GetFirstParent(Tag), Tag);
        });

        It(TEXT("MakePrimaryAssetIdFromTag should use the root parent as the type"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Leaf"));
            const auto AssetId = MakePrimaryAssetIdFromTag(Tag);

            TestEqual(TEXT("type"), AssetId.PrimaryAssetType, FName{TEXT("Test")});
            TestEqual(TEXT("name"), AssetId.PrimaryAssetName, FName{TEXT("Test.Common.Tag.Leaf")});
        });
    });

    Describe(TEXT("String conversion"), [this]
    {
        It(TEXT("TagToStringWithoutDot should strip the dots"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Leaf"));

            TestEqual(TEXT("no dots"), TagToStringWithoutDot(Tag), FUtf8String{TEXT("TestCommonTagLeaf")});
            TestEqual(TEXT("name form"), TagToNameWithoutDot(Tag), FName{TEXT("TestCommonTagLeaf")});
        });

        It(TEXT("TryGetTagFromString should parse a valid tag string"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.RoundTrip"));

            TestTrue(TEXT("round trip"), TryGetTagFromString(Tag.GetTagName().ToUtf8String()) == Tag);
        });

        It(TEXT("TryGetTagFromString should return an empty tag for an invalid string"), [this]
        {
            TestTrue(TEXT("invalid"), !TryGetTagFromString(FUtf8String{TEXT("Not.A.Tag")}).IsValid());
        });
    });

    Describe(TEXT("TryUpdateTagString"), [this]
    {
        It(TEXT("should fill an empty string with the tag name"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Sync"));
            FUtf8String Comment;

            const auto bUpdated = TryUpdateTagString(Comment, Tag);

            TestTrue(TEXT("updated"), bUpdated);
            TestEqual(TEXT("comment"), Comment, Tag.GetTagName().ToUtf8String());
        });

        It(TEXT("should replace a mismatched tag string"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Sync"));
            const auto OtherTag = AddTag(TEXT("Test.Common.Tag.Other"));
            FUtf8String Comment{OtherTag.GetTagName().ToUtf8String()};

            const auto bUpdated = TryUpdateTagString(Comment, Tag);

            TestTrue(TEXT("updated"), bUpdated);
            TestEqual(TEXT("comment"), Comment, Tag.GetTagName().ToUtf8String());
        });

        It(TEXT("should not touch a matching string"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Sync"));
            FUtf8String Comment{Tag.GetTagName().ToUtf8String()};

            const auto bUpdated = TryUpdateTagString(Comment, Tag);

            TestFalse(TEXT("not updated"), bUpdated);
            TestEqual(TEXT("comment"), Comment, Tag.GetTagName().ToUtf8String());
        });

        It(TEXT("should not touch an arbitrary comment string"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Tag.Sync"));
            FUtf8String Comment{TEXT("free form comment")};

            const auto bUpdated = TryUpdateTagString(Comment, Tag);

            TestFalse(TEXT("not updated"), bUpdated);
            TestEqual(TEXT("comment"), Comment, FUtf8String{TEXT("free form comment")});
        });
    });

    Describe(TEXT("Common parent"), [this]
    {
        It(TEXT("FindCommonParentTag should return the deepest common prefix"), [this]
        {
            const auto TagOne = AddTag(TEXT("Test.Common.Parent.A"));
            const auto TagTwo = AddTag(TEXT("Test.Common.Parent.B"));

            TestEqual(TEXT("common parent"), FindCommonParentTag(TagOne, TagTwo), AddTag(TEXT("Test.Common.Parent")));
        });

        It(TEXT("FindCommonParentTag should return the tag itself when equal"), [this]
        {
            const auto Tag = AddTag(TEXT("Test.Common.Parent.Same"));

            TestEqual(TEXT("self"), FindCommonParentTag(Tag, Tag), Tag);
        });

        It(TEXT("FindCommonParentTag should return the deepest common prefix when disjoint"), [this]
        {
            const auto TagOne = AddTag(TEXT("Test.Common.ParentOne"));
            const auto TagTwo = AddTag(TEXT("Test.Common.ParentTwo"));

            TestEqual(TEXT("common prefix"), FindCommonParentTag(TagOne, TagTwo), AddTag(TEXT("Test.Common")));
        });
    });

    Describe(TEXT("Query matching"), [this]
    {
        It(TEXT("IsTagQueryMatches should match a tag against the query"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Query.A"));
            const auto TagB = AddTag(TEXT("Test.Common.Query.B"));
            const auto Query = FGameplayTagQuery::MakeQuery_MatchAllTags(FGameplayTagContainer::CreateFromArray(
                TArray<FGameplayTag>{TagA, TagB}));

            FGameplayTagContainer MatchingTags;
            MatchingTags.AddTag(TagA);
            MatchingTags.AddTag(TagB);
            TestTrue(TEXT("matches"), IsTagQueryMatches(Query, MatchingTags));
            TestFalse(TEXT("not all tags with a single tag"), IsTagQueryMatches(Query, TagA));

            FGameplayTagContainer MissingTags;
            MissingTags.AddTag(TagA);
            TestFalse(TEXT("not enough tags"), IsTagQueryMatches(Query, MissingTags));
        });

        It(TEXT("IsTagQueryMatches should match an empty query against anything"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Query.Any"));

            TestTrue(TEXT("empty query"), IsTagQueryMatches(FGameplayTagQuery{}, TagA));
        });
    });

    Describe(TEXT("Collections"), [this]
    {
        It(TEXT("GetHashForTags should be stable for the same set"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Hash.A"));
            const auto TagB = AddTag(TEXT("Test.Common.Hash.B"));

            const TArray<FGameplayTag> TagsOne{TagA, TagB};
            const TArray<FGameplayTag> TagsTwo{TagA, TagB};

            TestEqual(TEXT("stable hash"), GetHashForTags(TagsOne), GetHashForTags(TagsTwo));
        });

        It(TEXT("ToTagContainer should contain exactly the given tags"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Container.A"));
            const auto TagB = AddTag(TEXT("Test.Common.Container.B"));
            const TArray<FGameplayTag> Tags{TagA, TagB};

            const auto Container = ToTagContainer(Tags);

            TestTrue(TEXT("has A"), Container.HasTagExact(TagA));
            TestTrue(TEXT("has B"), Container.HasTagExact(TagB));
            TestEqual(TEXT("count"), Container.Num(), 2);
        });

        It(TEXT("GetSingleTagContainer should contain exactly one tag"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Single.A"));

            const auto& Container = GetSingleTagContainer(TagA);

            TestEqual(TEXT("count"), Container.Num(), 1);
            TestTrue(TEXT("has the tag"), Container.HasTagExact(TagA));
        });
    });

    Describe(TEXT("FRemGameplayTagWithCategory"), [this]
    {
        It(TEXT("should compare by tag and convert to FGameplayTag"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Category.A"));
            const auto TagB = AddTag(TEXT("Test.Common.Category.B"));

            FRemGameplayTagWithCategory WithCategory{TagA};

            TestTrue(TEXT("equals tag"), WithCategory == TagA);
            TestTrue(TEXT("not equals other"), WithCategory != TagB);
            TestEqual(TEXT("get tag"), WithCategory.GetTag(), TagA);
            TestEqual(TEXT("converts"), static_cast<FGameplayTag>(WithCategory), TagA);
        });

        It(TEXT("should be usable as a TMap key"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Category.Map"));

            TMap<FRemGameplayTagWithCategory, int32> Map;
            Map.Add(FRemGameplayTagWithCategory{TagA}, 42);

            TestEqual(TEXT("lookup by tag"), Map[FRemGameplayTagWithCategory{TagA}], 42);
        });

        It(TEXT("SetTagAndCategory should update the tag"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Category.Set"));
            FRemGameplayTagWithCategory WithCategory;

            WithCategory.SetTagAndCategory(TagA);

            TestEqual(TEXT("updated tag"), WithCategory.GetTag(), TagA);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
