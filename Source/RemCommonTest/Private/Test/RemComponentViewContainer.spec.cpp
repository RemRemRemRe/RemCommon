// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Components/RemComponentViewContainer.h"
#include "Components/RemComponentViewContainer.inl"

#include "Misc/AutomationTest.h"

#include "Test/RemComponentOwnerTestStructs.h"
#include "Test/RemComponentViewContainerTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Common::Private::FRemComponentViewContainerTestComponent;
using Rem::Common::Private::FRemComponentViewContainerTestCounters;
using Rem::Common::Private::FRemComponentViewContainerTestOtherComponent;
using Rem::Common::Private::URemComponentOwnerTestObject;

namespace
{
struct FRemComponentViewContainerTestContext
{
    FRemComponentViewContainerInstance Instance;
    TArray<TInstancedStruct<FRemComponentViewBase>> Components;

    FRemComponentViewContainerTestContext()
    {
        FRemComponentViewContainerTestCounters::Reset();

        Components.Add(TInstancedStruct<FRemComponentViewBase>::Make<FRemComponentViewContainerTestComponent>());
        Instance.SetComponentsView(Components);
    }
};
}

DEFINE_SPEC(FRemComponentViewContainerTest, "Rem.Common.ComponentViewContainer",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemComponentViewContainerTest::Define()
{
    Describe(TEXT("Lifecycle"), [this]
    {
        It(TEXT("should initialize and uninitialize the components in order"), [this]
        {
            FRemComponentViewContainerTestContext Context;

            auto* Owner = NewObject<URemComponentOwnerTestObject>();
            check(Owner);

            Context.Instance.Initialize(*Owner);
            TestTrue(TEXT("initialized"), Context.Instance.IsInitialized());
            TestEqual(TEXT("initialize count"),
                FRemComponentViewContainerTestCounters::InitializeOrder.Num(), 1);
            TestEqual(TEXT("initialize index"),
                FRemComponentViewContainerTestCounters::InitializeOrder[0], 0);

            Context.Instance.Uninitialize();
            TestFalse(TEXT("uninitialized"), Context.Instance.IsInitialized());
            TestEqual(TEXT("uninitialize count"),
                FRemComponentViewContainerTestCounters::UninitializeOrder.Num(), 1);
        });
    });

    Describe(TEXT("Lookup"), [this]
    {
        It(TEXT("should find the component by type and index"), [this]
        {
            FRemComponentViewContainerTestContext Context;

            const auto View = Context.Instance.FindComponent<FRemComponentViewContainerTestComponent>();
            TestTrue(TEXT("component found"), View.IsValid());

            TestEqual(TEXT("component index"),
                Context.Instance.GetComponentIndex<FRemComponentViewContainerTestComponent>(), 0);
        });

        It(TEXT("should report an absent component type"), [this]
        {
            FRemComponentViewContainerTestContext Context;

            TestEqual(TEXT("no index"),
                Context.Instance.GetComponentIndex<FRemComponentViewContainerTestOtherComponent>(),
                INDEX_NONE);

            const auto View = Context.Instance.FindComponent<FRemComponentViewContainerTestOtherComponent>();
            TestFalse(TEXT("no view"), View.IsValid());
        });
    });

    Describe(TEXT("Instance data"), [this]
    {
        It(TEXT("should expose the component instance data"), [this]
        {
            FRemComponentViewContainerTestContext Context;

            auto* Owner = NewObject<URemComponentOwnerTestObject>();
            check(Owner);

            Context.Instance.Initialize(*Owner);

            const auto Data = Context.Instance
                .GetInstanceData<FRemComponentViewContainerTestComponent>(0);
            TestTrue(TEXT("data valid"), Data.IsValid());
            TestEqual(TEXT("default value"), Data->Value, 0);

            Data->Value = 42;
            const auto DataAgain = Context.Instance
                .GetInstanceData<FRemComponentViewContainerTestComponent>(0);
            TestEqual(TEXT("value round-trip"), DataAgain->Value, 42);
        });
    });

    Describe(TEXT("Ticking"), [this]
    {
        It(TEXT("should dispatch ticks to components that should tick"), [this]
        {
            FRemComponentViewContainerTestContext Context;

            URemComponentOwnerTestObject* Owner = NewObject<URemComponentOwnerTestObject>();
            check(Owner);

            Context.Instance.Initialize(*Owner);

            Context.Instance.ForEachComponent<FRemComponentViewBase>(
                [&Context](const FRemComponentViewBase& Component, const int32 Index, const UScriptStruct&)
                {
                    const FRemComponentViewBase::FContext TickContext{Rem::MakeNotNull(&Context.Instance), Index};
                    if (Component.ShouldTick(TickContext))
                    {
                        Component.Tick(TickContext, 0.5f);
                    }
                });

            TestEqual(TEXT("tick count"), FRemComponentViewContainerTestCounters::TickDeltas.Num(), 1);
            TestEqual(TEXT("tick delta"), FRemComponentViewContainerTestCounters::TickDeltas[0], 0.5f);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
