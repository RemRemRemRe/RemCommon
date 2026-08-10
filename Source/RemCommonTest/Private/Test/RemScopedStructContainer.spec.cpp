// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemScopedStructContainerTestStructs.h"

#include "Misc/AutomationTest.h"
#include "Struct/RemScopedStructContainer.h"
#include "Struct/RemScopedStructContainer.inl"

#if WITH_DEV_AUTOMATION_TESTS

int32 Rem::Struct::Private::FRemScopedStructContainerTestCounters::LiveTracked = 0;

using Rem::Struct::Private::FRemScopedStructContainerTestAligned;
using Rem::Struct::Private::FRemScopedStructContainerTestHeavy;
using Rem::Struct::Private::FRemScopedStructContainerTestOversized;
using Rem::Struct::Private::FRemScopedStructContainerTestTracked;
using Rem::Struct::Private::FRemScopedStructContainerTestTrivial;
using Rem::Struct::Private::FRemScopedStructContainerTestWideAligned;
using Rem::Struct::TScopedStructContainer;

DEFINE_SPEC(FRemScopedStructContainerTest, "Rem.Struct.ScopedStructContainer",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemScopedStructContainerTest::Define()
{
    Describe(TEXT("Emplace"), [this]
    {
        It(TEXT("should in-place construct and read back by index"), [this]
        {
            TScopedStructContainer<> Pool;
            const auto Trivial = Pool.Emplace<FRemScopedStructContainerTestTrivial>();
            Trivial->Number    = 42.0f;
            Trivial->Count     = 7;

            TestEqual(TEXT("Num"), Pool.Num(), 1);
            TestTrue(TEXT("script struct"),
                Pool[0].GetScriptStruct() == FRemScopedStructContainerTestTrivial::StaticStruct());
            const auto* ReadBack = Pool[0].GetPtr<FRemScopedStructContainerTestTrivial>();
            TestNotNull(TEXT("read back"), ReadBack);
            TestEqual(TEXT("Number"), ReadBack->Number, 42.0f);
            TestEqual(TEXT("Count"), ReadBack->Count, 7);

            // Const access path.
            const auto& ConstPool     = Pool;
            const auto* ConstReadBack = ConstPool[0].GetPtr<FRemScopedStructContainerTestTrivial>();
            TestNotNull(TEXT("const read back"), ConstReadBack);
            TestEqual(TEXT("const Number"), ConstReadBack->Number, 42.0f);
        });

        It(TEXT("should emplace with constructor arguments"), [this]
        {
            TScopedStructContainer<> Pool;
            const auto Heavy = Pool.Emplace<FRemScopedStructContainerTestHeavy>(FString(TEXT("Hello")), 5);
            TestEqual(TEXT("Text"), Heavy->Text, FString(TEXT("Hello")));
            TestEqual(TEXT("Values num"), Heavy->Values.Num(), 5);

            for (int32 Index = 0; Index < 5; ++Index)
            {
                Heavy->Values[Index] = Index * 2;
            }
            const auto* ReadBack = Pool[0].GetPtr<FRemScopedStructContainerTestHeavy>();
            TestNotNull(TEXT("read back"), ReadBack);
            TestEqual(TEXT("read back value"), ReadBack->Values[3], 6);
        });
    });

    Describe(TEXT("Add"), [this]
    {
        It(TEXT("should deep-copy the source struct"), [this]
        {
            TScopedStructContainer<> Pool;
            FRemScopedStructContainerTestHeavy Source(FString(TEXT("Original")), 3);
            Source.Values = {1, 2, 3};

            const FStructView View = Pool.Add(FConstStructView::Make(Source));
            TestEqual(TEXT("Num"), Pool.Num(), 1);
            auto* const Stored = View.GetPtr<FRemScopedStructContainerTestHeavy>();
            TestNotNull(TEXT("stored"), Stored);
            TestEqual(TEXT("copied text"), Stored->Text, FString(TEXT("Original")));

            // Mutating the container must not affect the source.
            Stored->Text      = TEXT("Changed");
            Stored->Values[0] = 99;
            TestEqual(TEXT("source text intact"), Source.Text, FString(TEXT("Original")));
            TestEqual(TEXT("source value intact"), Source.Values[0], 1);

            // Mutating the source after Add must not affect the container.
            Source.Text = TEXT("Mutated");
            TestEqual(TEXT("container text intact"), Stored->Text, FString(TEXT("Changed")));
        });
    });

    Describe(TEXT("Overflow"), [this]
    {
        It(TEXT("should keep views stable and preserve order across chunk overflow"), [this]
        {
            TScopedStructContainer<64, 128> Pool;

            // Capture a view to the first element BEFORE overflow.
            const auto First        = Pool.Emplace<FRemScopedStructContainerTestTrivial>();
            First->Number           = 1.0f;
            const auto* FirstMemory = Pool[0].GetMemory();

            // Overflow the 64-byte inline block with heavy elements.
            for (int32 Index = 0; Index < 50; ++Index)
            {
                (void)Pool.Emplace<FRemScopedStructContainerTestHeavy>(FString::Printf(TEXT("Item%d"), Index), Index);
            }
            TestEqual(TEXT("Num"), Pool.Num(), 51);

            // The first element must not have moved.
            TestTrue(TEXT("view memory stable"), Pool[0].GetMemory() == FirstMemory);
            const auto* FirstReadBack = Pool[0].GetPtr<FRemScopedStructContainerTestTrivial>();
            TestNotNull(TEXT("first read back"), FirstReadBack);
            TestEqual(TEXT("first content intact"), FirstReadBack->Number, 1.0f);

            // Order must be preserved: element i is the i-th added.
            for (int32 Index = 0; Index < 50; ++Index)
            {
                const auto* Heavy = Pool[Index + 1].GetPtr<FRemScopedStructContainerTestHeavy>();
                TestNotNull(TEXT("heavy read back"), Heavy);
                TestEqual(TEXT("order text"), Heavy->Text, FString::Printf(TEXT("Item%d"), Index));
                TestEqual(TEXT("order values num"), Heavy->Values.Num(), Index);
            }
        });

        It(TEXT("should align every element to its struct min alignment"), [this]
        {
            static_assert(alignof(FRemScopedStructContainerTestAligned) >= 16,
                "FQuat member should pin 16-byte alignment");

            TScopedStructContainer<64, 256> Pool;
            for (int32 Index = 0; Index < 16; ++Index)
            {
                (void)Pool.Emplace<FRemScopedStructContainerTestAligned>();
                (void)Pool.Emplace<FRemScopedStructContainerTestTrivial>();
            }

            for (FStructView View : Pool)
            {
                const auto Address = reinterpret_cast<uintptr_t>(View.GetMemory());
                TestTrue(TEXT("aligned to struct min alignment"),
                    Address % static_cast<uintptr_t>(View.GetScriptStruct()->GetMinAlignment()) == 0);
            }
        });
    });

    Describe(TEXT("Oversized elements"), [this]
    {
        It(TEXT("should place oversized elements in dedicated chunks and keep views stable"), [this]
        {
            TScopedStructContainer<64, 128> Pool;
            auto First = Pool.Emplace<FRemScopedStructContainerTestOversized>();
            First->Id  = 1;
            FMemory::Memset(First->Payload, 0xAB, sizeof(First->Payload));
            const auto* FirstMemory = Pool[0].GetMemory();

            for (int32 Index = 0; Index < 3; ++Index)
            {
                (void)Pool.Emplace<FRemScopedStructContainerTestOversized>();
            }
            const FRemScopedStructContainerTestOversized Source{};
            (void)Pool.Add(FConstStructView::Make(Source));
            TestEqual(TEXT("Num"), Pool.Num(), 5);

            // The first element must not have moved across dedicated-chunk allocations.
            TestTrue(TEXT("first view stable"), Pool[0].GetMemory() == FirstMemory);
            const auto* FirstReadBack = Pool[0].GetPtr<FRemScopedStructContainerTestOversized>();
            TestNotNull(TEXT("first read back"), FirstReadBack);
            TestEqual(TEXT("id intact"), FirstReadBack->Id, 1);
            TestEqual(TEXT("payload byte intact"), FirstReadBack->Payload[123], 0xAB);

            // Every element stays fully readable.
            for (FStructView View : Pool)
            {
                TestTrue(TEXT("valid view"), View.IsValid());
            }
        });

        It(TEXT("should align wide-aligned elements to their full alignment on Emplace"), [this]
        {
            static_assert(alignof(FRemScopedStructContainerTestWideAligned) >= 32,
                "alignas(32) member should pin 32-byte alignment");

            TScopedStructContainer<64, 128> Pool;
            for (int32 Index = 0; Index < 4; ++Index)
            {
                (void)Pool.Emplace<FRemScopedStructContainerTestWideAligned>();
            }
            for (FStructView View : Pool)
            {
                const auto Address = reinterpret_cast<uintptr_t>(View.GetMemory());
                TestTrue(TEXT("32-byte aligned"), Address % 32 == 0);
            }
        });
    });

    Describe(TEXT("Iteration"), [this]
    {
        It(TEXT("should iterate in insertion order via range-for and GetNum"), [this]
        {
            TScopedStructContainer<64, 256> Pool;
            for (int32 Index = 0; Index < 20; ++Index)
            {
                const auto Element = Pool.Emplace<FRemScopedStructContainerTestTrivial>();
                Element->Count     = Index;
            }

            int32 Iterated = 0;
            for (FStructView View : Pool)
            {
                const auto* Item = View.GetPtr<FRemScopedStructContainerTestTrivial>();
                TestNotNull(TEXT("item"), Item);
                TestEqual(TEXT("iteration order"), Item->Count, Iterated);
                ++Iterated;
            }
            TestEqual(TEXT("iterated count"), Iterated, Pool.Num());
            TestEqual(TEXT("GetNum interop"), GetNum(Pool), Pool.Num());

            const auto& ConstPool = Pool;
            int32 ConstIterated   = 0;
            for (FConstStructView View : ConstPool)
            {
                const auto* Item = View.GetPtr<FRemScopedStructContainerTestTrivial>();
                TestNotNull(TEXT("const item"), Item);
                TestEqual(TEXT("const iteration order"), Item->Count, ConstIterated);
                ++ConstIterated;
            }
            TestEqual(TEXT("const iterated count"), ConstIterated, Pool.Num());
        });

        It(TEXT("should support post-increment on iterators"), [this]
        {
            TScopedStructContainer<64, 256> Pool;
            for (int32 Index = 0; Index < 10; ++Index)
            {
                (void)Pool.Emplace<FRemScopedStructContainerTestTrivial>();
            }

            int32 Count = 0;
            for (auto It = Pool.begin(); It != Pool.end();)
            {
                const auto Before = It;
                It++;
                TestTrue(TEXT("post-increment returns previous position"), Before != It);
                ++Count;
            }
            TestEqual(TEXT("count"), Count, Pool.Num());
        });

        It(TEXT("should be empty on construction"), [this]
        {
            TScopedStructContainer<> Pool;
            TestEqual(TEXT("Num"), Pool.Num(), 0);
            TestTrue(TEXT("begin == end"), Pool.begin() == Pool.end());

            const auto& ConstPool = Pool;
            TestTrue(TEXT("const begin == end"), ConstPool.begin() == ConstPool.end());
        });
    });

    Describe(TEXT("Destruction"), [this]
    {
        It(TEXT("should destroy every element exactly once"), [this]
        {
            using Rem::Struct::Private::FRemScopedStructContainerTestCounters;
            TestEqual(TEXT("baseline"), FRemScopedStructContainerTestCounters::LiveTracked, 0);
            {
                // Source must OUTLIVE the container on purpose: the assertions below split
                // destruction into two phases (container destroyed → only Source alive → 1,
                // Source destroyed → 0). Moving Source into the Pool scope would destroy it
                // together with the container and break the intermediate assertion.
                // ReSharper disable once CppTooWideScope
                const FRemScopedStructContainerTestTracked Source{123};
                {
                    TScopedStructContainer<64, 256> Pool;
                    for (int32 Index = 0; Index < 40; ++Index)
                    {
                        (void)Pool.Emplace<FRemScopedStructContainerTestTracked>(Index);
                    }
                    TestEqual(TEXT("live after emplace"), FRemScopedStructContainerTestCounters::LiveTracked, 41);

                    // Add path: InitializeStruct constructs (+1), CopyScriptStruct copies.
                    const auto AddedView = Pool.Add(FConstStructView::Make(Source));
                    TestEqual(TEXT("live after add"), FRemScopedStructContainerTestCounters::LiveTracked, 42);

                    const auto* Stored = AddedView.GetPtr<FRemScopedStructContainerTestTracked>();
                    TestNotNull(TEXT("stored"), Stored);
                    TestEqual(TEXT("copied id"), Stored->Id, 123);
                }
                TestEqual(TEXT("live after container destroyed"), FRemScopedStructContainerTestCounters::LiveTracked,
                    1);
            }
            TestEqual(TEXT("live after source destroyed"), FRemScopedStructContainerTestCounters::LiveTracked, 0);
        });
    });

    Describe(TEXT("Consumer usage"), [this]
    {
        It(TEXT("should find and mutate a struct of the requested type via range-for"), [this]
        {
            // Producer: a mixed-struct call stack.
            TScopedStructContainer<128, 256> Stack;
            (void)Stack.Emplace<FRemScopedStructContainerTestTrivial>();
            auto Config = Stack.Emplace<FRemScopedStructContainerTestHeavy>(FString(TEXT("Config")), 2);
            Config->Values = {10, 20};
            (void)Stack.Emplace<FRemScopedStructContainerTestAligned>();

            // Consumer: locate the requested type, read, then write through the view.
            int32 FoundCount = 0;
            for (FStructView View : Stack)
            {
                if (auto* Found = View.GetPtr<FRemScopedStructContainerTestHeavy>())
                {
                    TestEqual(TEXT("read text"), Found->Text, FString(TEXT("Config")));
                    Found->Values[1] = 99;
                    ++FoundCount;
                }
            }
            TestEqual(TEXT("found exactly once"), FoundCount, 1);

            // The write is visible to the caller: the view points into container memory.
            const auto* Verify = Stack[1].GetPtr<FRemScopedStructContainerTestHeavy>();
            TestNotNull(TEXT("verify"), Verify);
            TestEqual(TEXT("write visible to caller"), Verify->Values[1], 99);
        });

        It(TEXT("should find a struct via FindStructView interop"), [this]
        {
            TScopedStructContainer<128, 256> Stack;
            (void)Stack.Emplace<FRemScopedStructContainerTestTrivial>();
            auto Config = Stack.Emplace<FRemScopedStructContainerTestHeavy>(FString(TEXT("Interop")), 3);
            Config->Values = {1, 2, 3};
            (void)Stack.Emplace<FRemScopedStructContainerTestAligned>();

            // The container overload: no intermediate view array needed.
            const auto Found = Rem::Struct::FindStructView<FRemScopedStructContainerTestHeavy>(Stack);
            TestTrue(TEXT("found"), Found.Get<0>().IsValid());
            TestEqual(TEXT("found index"), Found.Get<1>(), 1);

            Found.Get<0>().GetPtr<FRemScopedStructContainerTestHeavy>()->Values[0] = 42;
            const auto* Verify = Stack[1].GetPtr<FRemScopedStructContainerTestHeavy>();
            TestNotNull(TEXT("verify"), Verify);
            TestEqual(TEXT("write visible to caller"), Verify->Values[0], 42);

            // The const overload yields a read-only view.
            const auto& ConstStack = Stack;
            const auto FoundConst =
                Rem::Struct::FindConstStructView<FRemScopedStructContainerTestHeavy>(ConstStack);
            TestTrue(TEXT("const found"), FoundConst.Get<0>().IsValid());
            TestEqual(TEXT("const found index"), FoundConst.Get<1>(), 1);
            TestEqual(TEXT("const read"), FoundConst.Get<0>().GetPtr<FRemScopedStructContainerTestHeavy>()->Values[0],
                42);
        });

        It(TEXT("should support the stack-passing pattern across call layers"), [this]
        {
            // Layer 1 (entry): push the structs the call stack needs.
            TScopedStructContainer<256, 512> Stack;
            Stack.Emplace<FRemScopedStructContainerTestTrivial>()->Count = 7;
            auto Config = Stack.Emplace<FRemScopedStructContainerTestHeavy>(FString(TEXT("Layer")), 1);
            Config->Values = {5};

            // Layer 2 (middle): the container is passed down by reference, untouched.
            auto& StackRef = Stack;

            // Layer 3 (leaf): read the config and mutate it.
            std::invoke([&]
            {
                for (FStructView View : StackRef)
                {
                    if (auto* Found = View.GetPtr<FRemScopedStructContainerTestHeavy>())
                    {
                        TestEqual(TEXT("leaf read"), Found->Text, FString(TEXT("Layer")));
                        Found->Values[0] = 77;
                    }
                }
            });

            // Back at layer 1 the mutation is visible.
            const auto* Verify = Stack[1].GetPtr<FRemScopedStructContainerTestHeavy>();
            TestNotNull(TEXT("verify"), Verify);
            TestEqual(TEXT("leaf write visible at entry"), Verify->Values[0], 77);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
