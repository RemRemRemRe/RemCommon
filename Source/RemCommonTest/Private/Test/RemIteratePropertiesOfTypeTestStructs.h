// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

// generated.h must be included BEFORE the type declarations: GENERATED_BODY() expands
// to a macro (FID_..._GENERATED_BODY) that is defined here, so a trailing include would
// leave it undefined (C4430). UHT emits a matching forward declaration inside the
// namespace, so the include stays outside it.
#include "RemIteratePropertiesOfTypeTestStructs.generated.h"

namespace Rem::Property::Private
{
/**
 * Test-only nested struct: IteratePropertiesOfType must descend into its members.
 */
USTRUCT()
struct FRemIterateTestNested
{
    GENERATED_BODY()

    UPROPERTY()
    int32 NestedValue{};

    UPROPERTY()
    FString NestedText{};
};

/**
 * Test-only struct exercising every recursion path of IteratePropertiesOfType:
 * top-level scalars, nested structs, array/map/set containers, instanced struct
 * redirection and static arrays.
 */
USTRUCT()
struct FRemIterateTestData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Number{};

    UPROPERTY()
    FRemIterateTestNested Nested{};

    UPROPERTY()
    TArray<int32> Numbers{};

    UPROPERTY()
    TArray<FRemIterateTestNested> NestedArray{};

    UPROPERTY()
    TMap<int32, FString> Map{};

    UPROPERTY()
    TSet<int32> Set{};

    UPROPERTY()
    FInstancedStruct Instanced{};

    UPROPERTY()
    int32 StaticArray[3]{};
};

/**
 * Populates every member of FRemIterateTestData with known values.
 */
inline void PopulateTestData(FRemIterateTestData& Data)
{
    Data.Number = 1;

    Data.Nested.NestedValue = 2;
    Data.Nested.NestedText = TEXT("x");

    Data.Numbers.Add(10);
    Data.Numbers.Add(20);
    Data.Numbers.Add(30);

    FRemIterateTestNested FirstNested{};
    FirstNested.NestedValue = 3;
    FirstNested.NestedText = TEXT("a");
    Data.NestedArray.Add(FirstNested);

    FRemIterateTestNested SecondNested{};
    SecondNested.NestedValue = 4;
    SecondNested.NestedText = TEXT("b");
    Data.NestedArray.Add(SecondNested);

    Data.Map.Add(5, TEXT("five"));
    Data.Map.Add(6, TEXT("six"));

    Data.Set.Add(7);
    Data.Set.Add(8);

    FRemIterateTestNested InstancedValue{};
    InstancedValue.NestedValue = 9;
    InstancedValue.NestedText = TEXT("z");
    Data.Instanced.InitializeAs<FRemIterateTestNested>(InstancedValue);

    Data.StaticArray[0] = 11;
    Data.StaticArray[1] = 12;
    Data.StaticArray[2] = 13;
}
}
