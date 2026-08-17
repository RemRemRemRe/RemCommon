// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Components/RemComponentViewContainer.h"

#include "RemComponentViewContainerTestStructs.generated.h"

namespace Rem::Common::Private
{
/** Plain counters shared by the tracked test components; live in the test binary only. */
struct FRemComponentViewContainerTestCounters
{
    static TArray<int32> InitializeOrder;
    static TArray<int32> UninitializeOrder;
    static TArray<float> TickDeltas;

    static void Reset();
};

USTRUCT()
struct FRemComponentViewContainerTestInstanceData
{
    GENERATED_BODY()

    using ThisClass = FRemComponentViewContainerTestInstanceData;

    UPROPERTY()
    int32 Value{};
};

/**
 * Test component: records lifecycle calls and exposes instance data.
 */
USTRUCT()
struct FRemComponentViewContainerTestComponent : public FRemComponentViewBase
{
    GENERATED_BODY()

    using ThisClass = FRemComponentViewContainerTestComponent;
    using FInstanceDataType = FRemComponentViewContainerTestInstanceData;

    virtual FConstStructView GetInstanceDataType(const FContext& Context) const override;

    virtual void Initialize(const FContext& Context) const override;
    virtual bool ShouldTick(const FContext& Context) const override;
    virtual void Tick(const FContext& Context, float DeltaSeconds) const override;
    virtual void Uninitialize(const FContext& Context) const override;
};

/** Second component type: only used to verify absent-type lookups. */
USTRUCT()
struct FRemComponentViewContainerTestOtherComponent : public FRemComponentViewBase
{
    GENERATED_BODY()

    using ThisClass = FRemComponentViewContainerTestOtherComponent;
    using FInstanceDataType = FRemComponentViewContainerTestInstanceData;
};
}
