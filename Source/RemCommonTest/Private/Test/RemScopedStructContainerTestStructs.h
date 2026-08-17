// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// generated.h must be included BEFORE the type declarations: GENERATED_BODY() expands
// to a macro (FID_..._GENERATED_BODY) that is defined here, so a trailing include would
// leave it undefined (C4430). UHT emits a matching forward declaration inside the
// namespace, so the include stays outside it.
#include "RemScopedStructContainerTestStructs.generated.h"

// Test-only USTRUCT live in a namespace (bAllowUETypesInNamespaces, set by
// RemSharedModuleRules::Apply): they are internal to the test module and must not
// pollute the global scope.
//
// UHT restriction: USTRUCT/UPROPERTY must not be wrapped in #if WITH_DEV_AUTOMATION_TESTS
// (only WITH_EDITORONLY_DATA is allowed). The structs are therefore unguarded; they are
// only referenced from the guarded spec .cpp, so non-test builds carry no test code.

namespace Rem::Struct::Private
{
/** Plain counter shared by the tracked test struct; lives in the test binary only. */
struct FRemScopedStructContainerTestCounters
{
    static int32 LiveTracked;
};

USTRUCT()
struct FRemScopedStructContainerTestTrivial
{
    GENERATED_BODY()

    UPROPERTY()
    float Number{};

    UPROPERTY()
    int32 Count{};
};

USTRUCT()
struct FRemScopedStructContainerTestHeavy
{
    GENERATED_BODY()

    UPROPERTY()
    FString Text;

    UPROPERTY()
    TArray<int32> Values;

    FRemScopedStructContainerTestHeavy() = default;

    FRemScopedStructContainerTestHeavy(FString InText, const int32 InCount)
        : Text(MoveTemp(InText))
    {
        Values.SetNum(InCount);
    }
};

USTRUCT()
struct FRemScopedStructContainerTestAligned
{
    GENERATED_BODY()

    UPROPERTY()
    FQuat Rotation{ForceInitToZero};

    UPROPERTY()
    int32 Extra{};
};

/** Larger than any realistic OverflowChunkBytes: exercises the "element bigger
 * than a whole chunk" allocation path (one element per dedicated chunk). */
USTRUCT()
struct FRemScopedStructContainerTestOversized
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Id{};

    UPROPERTY()
    uint8 Payload[4096]{};
};

/** alignas(32) via a non-reflected member: Emplace aligns to alignof(T) = 32,
 * exercising the ChunkSlackBytes headroom that 16-byte BlockAlignment alone
 * cannot provide. */
USTRUCT()
struct FRemScopedStructContainerTestWideAligned
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Id{};

    alignas(32) uint8 Payload[4096]{};
};

/**
 * Counts its own construction/destruction in FRemScopedStructContainerTestCounters so tests
 * can assert exactly-once destruction. The default ctor writes a non-zero ID on purpose:
 * it keeps InitializeStruct on the Construct path (a zero-initialized struct would be
 * flagged STRUCT_ZeroConstructor and memory zeroed, breaking the counter symmetry).
 */
USTRUCT()
struct FRemScopedStructContainerTestTracked
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Id{};

    FRemScopedStructContainerTestTracked()
        : Id(-1)
    {
        ++FRemScopedStructContainerTestCounters::LiveTracked;
    }

    FRemScopedStructContainerTestTracked(const int32 InId)
        : Id(InId)
    {
        ++FRemScopedStructContainerTestCounters::LiveTracked;
    }

    FRemScopedStructContainerTestTracked(const FRemScopedStructContainerTestTracked& Other)
        : Id(Other.Id)
    {
        ++FRemScopedStructContainerTestCounters::LiveTracked;
    }

    FRemScopedStructContainerTestTracked(FRemScopedStructContainerTestTracked&& Other) noexcept
        : Id(Other.Id)
    {
        ++FRemScopedStructContainerTestCounters::LiveTracked;
    }

    FRemScopedStructContainerTestTracked& operator=(const FRemScopedStructContainerTestTracked&) = default;
    FRemScopedStructContainerTestTracked& operator=(FRemScopedStructContainerTestTracked&&)      = default;

    ~FRemScopedStructContainerTestTracked()
    {
        --FRemScopedStructContainerTestCounters::LiveTracked;
    }
};
}
