// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "GameplayTag/RemGameplayTagArray.h"

#include "Misc/AutomationTest.h"
#include "RemTestGameplayTags.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemGameplayTagArrayTest, "Rem.Common.GameplayTagArray",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemGameplayTagArrayTest::Define()
{
    const auto First  = Rem::Test::AddNativeTag(TEXT("Rem.Common.TagArray.First"));
    const auto Second = Rem::Test::AddNativeTag(TEXT("Rem.Common.TagArray.Second"));
    const auto Parent = Rem::Test::AddNativeTag(TEXT("Rem.Common.TagArray"));

    Describe(TEXT("FRemGameplayTagArray"), [this, First, Second, Parent]
    {
        It(TEXT("should default to an empty tag list"), [this]
        {
            const FRemGameplayTagArray Array{};

            TestTrue(TEXT("empty"), Array.Tags.IsEmpty());
            TestTrue(TEXT("empty container"), Array.ToTagContainer().IsEmpty());
        });

        It(TEXT("should convert to a tag container"), [this, First, Second, Parent]
        {
            FRemGameplayTagArray Array;
            Array.Tags.Add(First);
            Array.Tags.Add(Second);

            const auto Container = Array.ToTagContainer();

            TestEqual(TEXT("tag count"), Container.Num(), 2);
            TestTrue(TEXT("first present"), Container.HasTagExact(First));
            TestTrue(TEXT("second present"), Container.HasTagExact(Second));
            TestTrue(TEXT("parent hierarchy"), Container.HasTag(Parent));
        });

        It(TEXT("should expose the tags through the generated getter"), [this, First]
        {
            FRemGameplayTagArray Array;
            Array.Tags.Add(First);

            TestEqual(TEXT("getter count"), Array.Get().Num(), 1);
            TestEqual(TEXT("getter element"), Array.Get()[0], First);
        });

        // note: no CopyScriptStruct round-trip here - the Tags array member would
        // alias through the byte copy (same contract as RemStateTreeTest payloads)
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
