// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemTestGameplayTags.h"

#include "GameplayTagContainer.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemTestGameplayTagsTest, "Rem.Common.TestGameplayTags",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemTestGameplayTagsTest::Define()
{
    Describe(TEXT("AddNativeTag"), [this]
    {
        It(TEXT("should register a new native tag"), [this]
        {
            const auto Tag = Rem::Test::AddNativeTag(FName{TEXTVIEW("Rem.Test.GameplayTags.Fresh")});

            TestTrue(TEXT("valid"), Tag.IsValid());
            TestEqual(TEXT("name"), Tag.GetTagName(), FName{TEXTVIEW("Rem.Test.GameplayTags.Fresh")});
        });

        It(TEXT("should return the same tag instance when the tag already exists"), [this]
        {
            const auto First  = Rem::Test::AddNativeTag(FName{TEXTVIEW("Rem.Test.GameplayTags.Reused")});
            const auto Second = Rem::Test::AddNativeTag(FName{TEXTVIEW("Rem.Test.GameplayTags.Reused")});

            TestEqual(TEXT("same tag"), First, Second);
            TestEqual(TEXT("same name"), First.GetTagName(), FName{TEXTVIEW("Rem.Test.GameplayTags.Reused")});
        });

        It(TEXT("should register nested tags with their parent hierarchy"), [this]
        {
            const auto Parent = Rem::Test::AddNativeTag(FName{TEXTVIEW("Rem.Test.GameplayTags.Parent")});
            const auto Child  = Rem::Test::AddNativeTag(FName{TEXTVIEW("Rem.Test.GameplayTags.Parent.Child")});

            TestTrue(TEXT("parent valid"), Parent.IsValid());
            TestTrue(TEXT("child valid"), Child.IsValid());

            const auto Container = FGameplayTagContainer{Child};
            TestTrue(TEXT("child matches parent"), Container.HasTag(Parent));
            TestTrue(TEXT("child matches itself"), Container.HasTagExact(Child));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
