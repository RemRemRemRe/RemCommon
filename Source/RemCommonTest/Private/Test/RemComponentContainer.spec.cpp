// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemComponentContainerTestStructs.h"

#include "Components/RemComponentContainer.inl"
#include "Misc/AutomationTest.h"
#include "UObject/UObjectGlobals.h"

#if WITH_DEV_AUTOMATION_TESTS

TArray<int32> Rem::Common::Private::FRemComponentContainerTestCounters::InitializeOrder;
TArray<int32> Rem::Common::Private::FRemComponentContainerTestCounters::UninitializeOrder;
TArray<int32> Rem::Common::Private::FRemComponentContainerTestCounters::TickOrder;
TArray<float> Rem::Common::Private::FRemComponentContainerTestCounters::TickDeltas;
bool Rem::Common::Private::FRemComponentContainerTestCounters::bBShouldTick = false;

void Rem::Common::Private::FRemComponentContainerTestCounters::Reset()
{
    InitializeOrder.Reset();
    UninitializeOrder.Reset();
    TickOrder.Reset();
    TickDeltas.Reset();
    bBShouldTick = false;
}

void Rem::Common::Private::FRemComponentContainerTestA::Initialize(const FContext& Context)
{
    FRemComponentContainerTestCounters::InitializeOrder.Add(Context.ComponentIndex);
}

void Rem::Common::Private::FRemComponentContainerTestA::Tick(const FContext& Context, const float DeltaSeconds)
{
    FRemComponentContainerTestCounters::TickOrder.Add(Context.ComponentIndex);
    FRemComponentContainerTestCounters::TickDeltas.Add(DeltaSeconds);
}

void Rem::Common::Private::FRemComponentContainerTestA::Uninitialize(const FContext& Context)
{
    FRemComponentContainerTestCounters::UninitializeOrder.Add(Context.ComponentIndex);
}

void Rem::Common::Private::FRemComponentContainerTestB::Initialize(const FContext& Context)
{
    FRemComponentContainerTestCounters::InitializeOrder.Add(Context.ComponentIndex);
}

bool Rem::Common::Private::FRemComponentContainerTestB::ShouldTick(const FContext&) const
{
    return FRemComponentContainerTestCounters::bBShouldTick;
}

void Rem::Common::Private::FRemComponentContainerTestB::Tick(const FContext& Context, const float DeltaSeconds)
{
    FRemComponentContainerTestCounters::TickOrder.Add(Context.ComponentIndex);
    FRemComponentContainerTestCounters::TickDeltas.Add(DeltaSeconds);
}

void Rem::Common::Private::FRemComponentContainerTestB::Uninitialize(const FContext& Context)
{
    FRemComponentContainerTestCounters::UninitializeOrder.Add(Context.ComponentIndex);
}

using Rem::Common::Private::ERemComponentContainerTestSlot;
using Rem::Common::Private::URemComponentContainerTestOwner;
using Rem::Common::Private::FRemComponentContainerTestA;
using Rem::Common::Private::FRemComponentContainerTestB;
using Rem::Common::Private::FRemComponentContainerTestCounters;

DEFINE_SPEC(FRemComponentContainerTest, "Rem.Common.ComponentContainer",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemComponentContainerTest::Define()
{
    const auto Setup = [](FRemComponentContainer& Container)
    {
        TArray<TInstancedStruct<FRemComponentBase>> Components;
        Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>());
        Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestB>());
        Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>());
        Container.SetComponentsView(std::move(Components));
    };

    Describe(TEXT("Initialize"), [this, Setup]
    {
        It(TEXT("should dispatch Initialize to every component in container order"), [this, Setup]
        {
            FRemComponentContainerTestCounters::Reset();

            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);

            TestEqual(TEXT("initialize order"), FRemComponentContainerTestCounters::InitializeOrder,
                TArray<int32>({0, 1, 2}));
            TestTrue(TEXT("is initialized"), Container.IsInitialized());
        });

        It(TEXT("should expose the owner via GetOwner after Initialize"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);

            TestEqual(TEXT("owner"), Container.GetOwner<URemComponentContainerTestOwner>(), Owner);
        });

        It(TEXT("should no-op when TryInitialize is called twice"), [this, Setup]
        {
            FRemComponentContainerTestCounters::Reset();

            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.TryInitialize(*Owner);
            Container.TryInitialize(*Owner);

            TestEqual(TEXT("initialize dispatched once"), FRemComponentContainerTestCounters::InitializeOrder.Num(), 3);
            TestTrue(TEXT("is initialized"), Container.IsInitialized());
        });
    });

    Describe(TEXT("Uninitialize"), [this, Setup]
    {
        It(TEXT("should dispatch Uninitialize to every component"), [this, Setup]
        {
            FRemComponentContainerTestCounters::Reset();

            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);
            Container.Uninitialize();

            const auto& Order = FRemComponentContainerTestCounters::UninitializeOrder;
            TestEqual(TEXT("uninitialize count"), Order.Num(), 3);
            if (Order.Num() == 3)
            {
                TestEqual(TEXT("first index"), Order[0], 0);
                TestEqual(TEXT("second index"), Order[1], 1);
                TestEqual(TEXT("third index"), Order[2], 2);
            }
            TestFalse(TEXT("not initialized"), Container.IsInitialized());
        });

        It(TEXT("should clear the owner on Uninitialize"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);
            Container.Uninitialize();

            TestEqual(TEXT("owner cleared"), Container.GetOwner<URemComponentContainerTestOwner>(),
                static_cast<URemComponentContainerTestOwner*>(nullptr));
        });

        It(TEXT("should no-op when TryUninitialize is called while not initialized"), [this, Setup]
        {
            FRemComponentContainerTestCounters::Reset();

            FRemComponentContainer Container;
            Setup(Container);

            Container.TryUninitialize();

            TestEqual(TEXT("no dispatch"), FRemComponentContainerTestCounters::UninitializeOrder.Num(), 0);
            TestFalse(TEXT("not initialized"), Container.IsInitialized());
        });
    });

    Describe(TEXT("FindComponent"), [this, Setup]
    {
        It(TEXT("should find the first component of a type"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            const auto A = Container.FindComponent<FRemComponentContainerTestA>();
            TestTrue(TEXT("A found"), A.IsValid());
            TestEqual(TEXT("A index"), Container.GetComponentIndex<FRemComponentContainerTestA>(), 0);

            const auto B = Container.FindComponent<FRemComponentContainerTestB>();
            TestTrue(TEXT("B found"), B.IsValid());
        });

        It(TEXT("should return an invalid view for a missing type"), [this, Setup]
        {
            FRemComponentContainer Container;
            TArray<TInstancedStruct<FRemComponentBase>> Components;
            Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>());
            Container.SetComponentsView(std::move(Components));

            const auto B = Container.FindComponent<FRemComponentContainerTestB>();
            TestFalse(TEXT("B not found"), B.IsValid());
        });

        It(TEXT("should find a component by index"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            const auto AtOne = Container.FindComponent<FRemComponentContainerTestB>(1);
            TestTrue(TEXT("index 1 is B"), AtOne.IsValid());

            const auto AtZero = Container.FindComponent<FRemComponentContainerTestA>(0);
            TestTrue(TEXT("index 0 is A"), AtZero.IsValid());
            AtZero->Value = 42;
            TestEqual(TEXT("write through view"), AtZero->Value, 42);
        });

        It(TEXT("should find a component by scoped enum index"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            const auto A = Container.FindComponent<FRemComponentContainerTestA>(ERemComponentContainerTestSlot::ThirdA);
            TestTrue(TEXT("enum index 2 is A"), A.IsValid());

            const auto B = Container.FindComponent<FRemComponentContainerTestB>(ERemComponentContainerTestSlot::SecondB);
            TestTrue(TEXT("enum index 1 is B"), B.IsValid());
        });

        It(TEXT("should support const lookup"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);
            const auto& ConstContainer = Container;

            const auto A = ConstContainer.FindComponent<FRemComponentContainerTestA>();
            TestTrue(TEXT("const A found"), A.IsValid());
        });
    });

    Describe(TEXT("ForEachComponent"), [this, Setup]
    {
        It(TEXT("should iterate all components with index and script struct"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            int32 Iterated = 0;
            Container.ForEachComponent<FRemComponentBase>(
                [&](const FRemComponentBase& Component, const int32 Index, const UScriptStruct& ScriptStruct)
                {
                    TestEqual(TEXT("index"), Index, Iterated);
                    TestTrue(TEXT("script struct is a component type"),
                        ScriptStruct.IsChildOf(FRemComponentBase::StaticStruct()));
                    TestTrue(TEXT("view points at component"), &Component != nullptr);
                    ++Iterated;
                });

            TestEqual(TEXT("iterated count"), Iterated, 3);
        });

        It(TEXT("should iterate a typed subset"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            int32 ACount = 0;
            Container.ForEachComponent<FRemComponentContainerTestA>(
                [&](FRemComponentContainerTestA& Component, const int32, const UScriptStruct&)
                {
                    Component.Value = ACount;
                    ++ACount;
                });

            TestEqual(TEXT("two A components"), ACount, 2);
            const auto First = Container.FindComponent<FRemComponentContainerTestA>();
            TestEqual(TEXT("first A value"), First->Value, 0);
        });
    });

    Describe(TEXT("GetComponentIndex"), [this, Setup]
    {
        It(TEXT("should return the index of the first exact type match"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            TestEqual(TEXT("first A"), Container.GetComponentIndex<FRemComponentContainerTestA>(), 0);
            TestEqual(TEXT("first B"), Container.GetComponentIndex<FRemComponentContainerTestB>(), 1);
        });

        It(TEXT("should return INDEX_NONE for a missing type"), [this, Setup]
        {
            FRemComponentContainer Container;
            TArray<TInstancedStruct<FRemComponentBase>> Components;
            Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>());
            Container.SetComponentsView(std::move(Components));

            TestEqual(TEXT("B missing"), Container.GetComponentIndex<FRemComponentContainerTestB>(), INDEX_NONE);
        });

        It(TEXT("should return the index of a given component instance"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            const auto B = Container.FindComponent<FRemComponentContainerTestB>();
            TestEqual(TEXT("instance index"), Container.GetComponentIndex(*B.GetPtr()), 1);
        });
    });

    Describe(TEXT("SetComponentsView"), [this, Setup]
    {
        It(TEXT("should replace components from an rvalue array"), [this]
        {
            FRemComponentContainer Container;

            TArray<TInstancedStruct<FRemComponentBase>> Components;
            Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>());
            Container.SetComponentsView(std::move(Components));

            TestTrue(TEXT("A found"), Container.FindComponent<FRemComponentContainerTestA>().IsValid());
            TestFalse(TEXT("B not found"), Container.FindComponent<FRemComponentContainerTestB>().IsValid());
        });

        It(TEXT("should replace components from a const struct view array"), [this]
        {
            FRemComponentContainer Container;

            TInstancedStruct<FRemComponentBase> Instance =
                TInstancedStruct<FRemComponentBase>::Make<FRemComponentContainerTestA>();
            TArray<TConstStructView<FRemComponentBase>> Views;
            Views.Add(TConstStructView<FRemComponentBase>(Instance));
            Container.SetComponentsView(Views);

            TestTrue(TEXT("A found"), Container.FindComponent<FRemComponentContainerTestA>().IsValid());
        });

        It(TEXT("should uninitialize previous components when replacing"), [this, Setup]
        {
            FRemComponentContainerTestCounters::Reset();

            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);

            TArray<TInstancedStruct<FRemComponentBase>> Empty;
            Container.SetComponentsView(std::move(Empty));

            const auto& Order = FRemComponentContainerTestCounters::UninitializeOrder;
            TestEqual(TEXT("uninitialize count"), Order.Num(), 3);
            if (Order.Num() == 3)
            {
                TestEqual(TEXT("first index"), Order[0], 0);
                TestEqual(TEXT("second index"), Order[1], 1);
                TestEqual(TEXT("third index"), Order[2], 2);
            }
            TestFalse(TEXT("not initialized"), Container.IsInitialized());
        });
    });

    Describe(TEXT("Owner access"), [this, Setup]
    {
        It(TEXT("should expose the owner via the component context"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            auto* Owner = NewObject<URemComponentContainerTestOwner>();
            Container.Initialize(*Owner);

            const FRemComponentBase::FContext Context{&Container, 0};
            TestEqual(TEXT("owner from context"), FRemComponentBase::GetOwner<URemComponentContainerTestOwner>(Context), Owner);
        });
    });

    Describe(TEXT("Reflection copy"), [this, Setup]
    {
        It(TEXT("should deep-copy components via CopyScriptStruct without double-free"), [this, Setup]
        {
            FRemComponentContainer Container;
            Setup(Container);

            auto A = Container.FindComponent<FRemComponentContainerTestA>();
            A->Value = 7;

            FRemComponentContainer Destination;
            FRemComponentContainer::StaticStruct()->CopyScriptStruct(&Destination, &Container);

            const auto Copied = Destination.FindComponent<FRemComponentContainerTestA>();
            TestTrue(TEXT("copied A found"), Copied.IsValid());
            TestEqual(TEXT("copied value"), Copied->Value, 7);
            TestTrue(TEXT("B copied"), Destination.FindComponent<FRemComponentContainerTestB>().IsValid());

            // Both containers destruct at scope end: no double-free proves the copy is deep.
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
