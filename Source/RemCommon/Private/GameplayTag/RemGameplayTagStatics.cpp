// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "GameplayTag/RemGameplayTagStatics.h"

#include "GameplayTagContainer.h"
#include "GameplayTag/RemGameplayTagArray.h"
#include "GameplayTagsManager.h"
#include "RemCommonLog.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemLogMacros.h"
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

FUtf8String TagToStringWithoutDot(const FGameplayTag& Tag)
{
    return FUtf8String(Tag.GetTagName().ToString().Replace(TEXT("."), TEXT("")));
}

FName TagToNameWithoutDot(const FGameplayTag& Tag)
{
    return FName{TagToStringWithoutDot(Tag)};
}

FGameplayTag TryGetTagFromString(const FUtf8String& TagString)
{
    if (auto& Manager = UGameplayTagsManager::Get();
        Manager.IsValidGameplayTagString(StringCast<FStringView::ElementType>(*TagString)))
    {
        const auto Tag =
            FGameplayTag::RequestGameplayTag(FName{TagString.Len(), GetData(TagString), FNAME_Find}, false);
        return Tag;
    }
    return FGameplayTag::EmptyTag;
}

bool TryUpdateTagString(FUtf8String& StringRef, const FGameplayTag& Tag)
{
    if (StringRef.IsEmpty())
    {
        // empty string, use tag name string by default
        StringRef = Tag.GetTagName().ToUtf8String();
    }
    else if (const auto TagFromComment = TryGetTagFromString(StringRef);
        TagFromComment.IsValid() && TagFromComment != Tag)
    {
        // used tag name string but not matched
        StringRef = Tag.GetTagName().ToUtf8String();
    }
    else
    {
        // did nothing
        return false;
    }

    // updated
    return true;
}

bool TryUpdateTagString(FString& StringRef, const FGameplayTag& Tag)
{
    FUtf8String Utf8String{StringRef};
    ON_SCOPE_EXIT
    {
        StringRef = FString{Utf8String};
    };
    return TryUpdateTagString(Utf8String, Tag);
}

TArray<FUtf8String> GetTagsString(const TConstArrayView<FGameplayTag> Tags)
{
    TArray<FUtf8String> TagsString{};
    TagsString.Reserve(Tags.Num());

    for (auto& TagOne : Tags)
    {
        TagsString.Add(TagOne.GetTagName().ToUtf8String());
    }

    return TagsString;
}

FGameplayTagContainer ToTagContainer(const TConstArrayView<FGameplayTag> Tags)
{
    return FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>(Tags));
}

FUtf8String ToString(const TConstArrayView<FGameplayTag> Tags, const bool bQuoted)
{
    TUtf8StringBuilder<256> Builder;
    for (auto Index = 0; Index < Tags.Num(); ++Index)
    {
        if (bQuoted)
        {
            Builder << '"';
        }
        Builder << Tags[Index].ToString();
        if (bQuoted)
        {
            Builder << '"';
        }

        if (Index < Tags.Num() - 1)
        {
            Builder << ", ";
        }
    }
    return FUtf8String(Builder);
}

FGameplayTag FindCommonParentTag(const FGameplayTag& TagOne, const FGameplayTag& TagTwo)
{
    RemCheckVariable(TagOne, return {});
    RemCheckVariable(TagTwo, return {});

    // the string overload is the single implementation, so the two overloads cannot disagree about equal tags
    const auto StringOne = TagOne.GetTagName().ToUtf8String();
    const auto StringTwo = TagTwo.GetTagName().ToUtf8String();

    return FindCommonParentTag(StringOne, StringTwo);
}

FGameplayTag FindCommonParentTag(const FUtf8StringView TagStringOne, const FUtf8StringView TagStringTwo)
{
    // the deepest tag common to both: themselves when equal, the ancestor when one contains the other, the shared
    // dotted prefix otherwise
    // @note an equal pair used to come back as its parent here (the scan below only looks at the characters before
    // the first difference) while the tag overload returned the pair itself
    if (TagStringOne.IsEmpty() || TagStringTwo.IsEmpty())
    {
        return {};
    }

    if (TagStringOne == TagStringTwo)
    {
        return FGameplayTag::RequestGameplayTag(FName(TagStringOne.Len(), TagStringOne.GetData(), FNAME_Find));
    }

    // ancestor case: the shorter view is a strict prefix and the longer one continues with a dot, which the
    // dotted-prefix scan below cannot see because the two views are equal up to the shorter one's end
    const auto bFirstIsShorter    = TagStringOne.Len() < TagStringTwo.Len();
    const FUtf8StringView Shorter = bFirstIsShorter
                                        ? TagStringOne
                                        : TagStringTwo;
    const FUtf8StringView Longer = bFirstIsShorter
                                       ? TagStringTwo
                                       : TagStringOne;

    if (Longer.Len() > Shorter.Len() && Longer.StartsWith(Shorter) && Longer[Shorter.Len()] == '.')
    {
        return FGameplayTag::RequestGameplayTag(FName(Shorter.Len(), Shorter.GetData(), FNAME_Find));
    }

    const auto MinLength = FMath::Min(TagStringOne.Len(), TagStringTwo.Len());
    int32 DotIndex{INDEX_NONE};

    for (auto Index = 0; Index < MinLength; ++Index)
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

    // the length-taking FName form: a substring view is not NUL-terminated at its length
    const auto CommonPrefix = TagStringOne.SubStr(0, DotIndex);

    return FGameplayTag::RequestGameplayTag(FName(CommonPrefix.Len(), CommonPrefix.GetData(), FNAME_Find));
}

TArray<FGameplayTag> FindCommonParentTags(const TConstArrayView<FGameplayTag> TagsOne,
    const TConstArrayView<FGameplayTag> TagsTwo)
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

const FGameplayTagContainer& GetSingleTagContainer(const FGameplayTag& Tag)
{
    if (const auto TagNode = UGameplayTagsManager::Get().FindTagNode(Tag);
        TagNode.IsValid())
    {
        return TagNode->GetSingleTagContainer();
    }

    // This tag should always be invalid if the node is missing
    RemCheckVariable(Tag,
        {
        REM_LOG_FUNCTION(LogRemCommon, Error,
            "GetSingleTagContainer passed invalid gameplay tag {0}, only registered tags can be queried", Tag);
        });

    return FGameplayTagContainer::EmptyContainer;
}

TConstArrayView<FGameplayTag> GetTagArrayView(const FGameplayTagContainer& Container)
{
    return Container.GetGameplayTagArray();
}

TConstArrayView<FGameplayTag> GetTagArrayView(const FGameplayTag& Tag)
{
    return MakeArrayView(&Tag, 1);
}

TConstArrayView<FGameplayTag> GetTagArrayView(const FGameplayTagQuery& Query)
{
    return Query.GetGameplayTagArray();
}

TConstArrayView<FGameplayTag> GetTagArrayView(const FRemGameplayTagArray& Array)
{
    return Array.Get();
}

TConstArrayView<FGameplayTag> GetTagArrayView(const TArray<FGameplayTag>& Array)
{
    return Array;
}
}
