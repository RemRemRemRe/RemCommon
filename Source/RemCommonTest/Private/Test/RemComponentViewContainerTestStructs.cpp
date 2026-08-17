// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemComponentViewContainerTestStructs.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentViewContainerTestStructs)

namespace Rem::Common::Private
{
TArray<int32> FRemComponentViewContainerTestCounters::InitializeOrder;
TArray<int32> FRemComponentViewContainerTestCounters::UninitializeOrder;
TArray<float> FRemComponentViewContainerTestCounters::TickDeltas;

void FRemComponentViewContainerTestCounters::Reset()
{
    InitializeOrder.Reset();
    UninitializeOrder.Reset();
    TickDeltas.Reset();
}

FConstStructView FRemComponentViewContainerTestComponent::GetInstanceDataType(const FContext& Context) const
{
    // type-only view (null memory): InsertAt default-constructs the instance data
    return FConstStructView{ FInstanceDataType::StaticStruct() };
}

void FRemComponentViewContainerTestComponent::Initialize(const FContext& Context) const
{
    FRemComponentViewContainerTestCounters::InitializeOrder.Add(Context.ComponentIndex);
}

bool FRemComponentViewContainerTestComponent::ShouldTick(const FContext& Context) const
{
    return true;
}

void FRemComponentViewContainerTestComponent::Tick(const FContext& Context, const float DeltaSeconds) const
{
    FRemComponentViewContainerTestCounters::TickDeltas.Add(DeltaSeconds);
}

void FRemComponentViewContainerTestComponent::Uninitialize(const FContext& Context) const
{
    FRemComponentViewContainerTestCounters::UninitializeOrder.Add(Context.ComponentIndex);
}
}
