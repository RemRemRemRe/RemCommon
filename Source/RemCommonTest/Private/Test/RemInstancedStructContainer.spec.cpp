// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemScopedStructContainerTestStructs.h"

#include "Misc/AutomationTest.h"
#include "Struct/RemInstancedStructContainer.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Struct::Private::FRemScopedStructContainerTestHeavy;
using Rem::Struct::Private::FRemScopedStructContainerTestTrivial;

DEFINE_SPEC(FRemInstancedStructContainerTest, "Rem.Struct.InstancedStructContainer",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemInstancedStructContainerTest::Define()
{
    Describe(TEXT("TryAdd"), [this]
    {
        It(TEXT("should add a new key and retrieve it typed"), [this]
        {
            FRemInstancedStructContainer Container;
            constexpr auto Key = static_cast<FRemInstancedStructContainer::FKeyType>(1);

            FRemScopedStructContainerTestTrivial Source{};
            Source.Number = 3.5f;
            Source.Count  = 9;

            TestTrue(TEXT("add"), Container.TryAdd(Key, FConstStructView::Make(Source)));

            const auto View = Container.TryGetView<FRemScopedStructContainerTestTrivial>(Key);
            TestTrue(TEXT("view valid"), View.IsValid());
            TestEqual(TEXT("number"), View->Number, 3.5f);
            TestEqual(TEXT("count"), View->Count, 9);
        });

        It(TEXT("should fail to add a duplicate key and keep the first entry"), [this]
        {
            FRemInstancedStructContainer Container;
            constexpr auto Key = static_cast<FRemInstancedStructContainer::FKeyType>(1);

            FRemScopedStructContainerTestTrivial FirstSource;
            FirstSource.Number = 1.0f;

            FRemScopedStructContainerTestHeavy SecondSource(TEXT("Second"), 2);

            TestTrue(TEXT("first add"), Container.TryAdd(Key, FConstStructView::Make(FirstSource)));
            TestFalse(TEXT("duplicate add rejected"), Container.TryAdd(Key, FConstStructView::Make(SecondSource)));

            const auto View = Container.TryGetView<FRemScopedStructContainerTestTrivial>(Key);
            TestTrue(TEXT("first entry kept"), View.IsValid());
            TestEqual(TEXT("first number"), View->Number, 1.0f);
        });
    });

    Describe(TEXT("Contains"), [this]
    {
        It(TEXT("should report the presence of a key"), [this]
        {
            FRemInstancedStructContainer Container;
            constexpr auto Key = static_cast<FRemInstancedStructContainer::FKeyType>(7);

            TestFalse(TEXT("absent"), Container.Contains(Key));

            FRemScopedStructContainerTestTrivial Source{};
            Container.TryAdd(Key, FConstStructView::Make(Source));

            TestTrue(TEXT("present"), Container.Contains(Key));
        });
    });

    Describe(TEXT("Reset and Empty"), [this]
    {
        It(TEXT("should clear all entries on Reset"), [this]
        {
            FRemInstancedStructContainer Container;
            constexpr auto Key = static_cast<FRemInstancedStructContainer::FKeyType>(1);

            FRemScopedStructContainerTestTrivial Source{};
            Container.TryAdd(Key, FConstStructView::Make(Source));
            TestTrue(TEXT("added"), Container.Contains(Key));

            Container.Reset();

            TestFalse(TEXT("cleared"), Container.Contains(Key));
            TestTrue(TEXT("no view"), !Container.TryGetView(Key).IsValid());
        });

        It(TEXT("should clear all entries on Empty"), [this]
        {
            FRemInstancedStructContainer Container;
            constexpr auto Key = static_cast<FRemInstancedStructContainer::FKeyType>(2);

            FRemScopedStructContainerTestTrivial Source{};
            Container.TryAdd(Key, FConstStructView::Make(Source));
            TestTrue(TEXT("added"), Container.Contains(Key));

            Container.Empty();

            TestFalse(TEXT("cleared"), Container.Contains(Key));
            TestTrue(TEXT("no view"), !Container.TryGetView(Key).IsValid());
        });
    });

    Describe(TEXT("Type hash key pattern"), [this]
    {
        It(TEXT("should support keying by struct type hash"), [this]
        {
            FRemInstancedStructContainer Container;

            const auto TrivialKey = static_cast<FRemInstancedStructContainer::FKeyType>(
                GetTypeHash(FRemScopedStructContainerTestTrivial::StaticStruct()->GetFName()));
            const auto HeavyKey = static_cast<FRemInstancedStructContainer::FKeyType>(
                GetTypeHash(FRemScopedStructContainerTestHeavy::StaticStruct()->GetFName()));

            FRemScopedStructContainerTestTrivial TrivialSource;
            TrivialSource.Count = 5;
            FRemScopedStructContainerTestHeavy HeavySource(TEXT("Heavy"), 3);

            Container.TryAdd(TrivialKey, FConstStructView::Make(TrivialSource));
            Container.TryAdd(HeavyKey, FConstStructView::Make(HeavySource));

            const auto Trivial = Container.TryGetView<FRemScopedStructContainerTestTrivial>(TrivialKey);
            const auto Heavy   = Container.TryGetView<FRemScopedStructContainerTestHeavy>(HeavyKey);

            TestTrue(TEXT("trivial by type hash"), Trivial.IsValid());
            TestEqual(TEXT("trivial count"), Trivial->Count, 5);
            TestTrue(TEXT("heavy by type hash"), Heavy.IsValid());
            TestEqual(TEXT("heavy text"), Heavy->Text, FString{TEXT("Heavy")});
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
