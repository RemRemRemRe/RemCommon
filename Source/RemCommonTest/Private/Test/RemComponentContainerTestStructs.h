// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Components/RemComponentContainer.h"

// generated.h must be included BEFORE the type declarations: GENERATED_BODY() expands
// to a macro (FID_..._GENERATED_BODY) that is defined here, so a trailing include would
// leave it undefined (C4430). UHT emits a matching forward declaration inside the
// namespace, so the include stays outside it.
#include "RemComponentContainerTestStructs.generated.h"

// Test-only USTRUCT live in a namespace (bAllowUETypesInNamespaces, set by
// RemSharedModuleRules::Apply): they are internal to the test module and must not
// pollute the global scope.
//
// UHT restriction: USTRUCT/UPROPERTY must not be wrapped in #if WITH_DEV_AUTOMATION_TESTS
// (only WITH_EDITORONLY_DATA is allowed). The structs are therefore unguarded; they are
// only referenced from the guarded spec .cpp, so non-test builds carry no test code.

namespace Rem::Common::Private
{
/** Plain counters shared by the tracked test components; live in the test binary only. */
struct FRemComponentContainerTestCounters
{
    static TArray<int32> InitializeOrder;
    static TArray<int32> UninitializeOrder;
    static TArray<int32> TickOrder;
    static TArray<float> TickDeltas;
    static bool bBShouldTick;

    static void Reset();
};

enum class ERemComponentContainerTestSlot : uint8
{
    FirstA  = 0,
    SecondB = 1,
    ThirdA  = 2,
};

/** Concrete owner class for container lifecycle tests (UObject itself is CLASS_Abstract in UE5). */
UCLASS()
class URemComponentContainerTestOwner : public UObject
{
    GENERATED_BODY()
};

USTRUCT()
struct FRemComponentContainerTestA : public FRemComponentBase
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Value{};

    virtual void Initialize(const FContext& Context) override;
    virtual void Tick(const FContext& Context, float DeltaSeconds) override;
    virtual void Uninitialize(const FContext& Context) override;
};

USTRUCT()
struct FRemComponentContainerTestB : public FRemComponentBase
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name{};

    virtual void Initialize(const FContext& Context) override;
    virtual bool ShouldTick(const FContext& Context) const override;
    virtual void Tick(const FContext& Context, float DeltaSeconds) override;
    virtual void Uninitialize(const FContext& Context) override;
};
}
