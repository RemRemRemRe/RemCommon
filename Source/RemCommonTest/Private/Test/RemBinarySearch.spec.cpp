// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemBinarySearch.h"

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
TArray<int32> MakeSortedArray(const int32 Count)
{
    TArray<int32> Result;
    Result.Reserve(Count);
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Result.Add(Index * 2);
    }
    return Result;
}
}

DEFINE_SPEC(FRemBinarySearchTest, "Rem.Common.BinarySearch",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemBinarySearchTest::Define()
{
    Describe(TEXT("LowerBound"), [this]
    {
        It(TEXT("should find the first element not less than the value"), [this]
        {
            auto Numbers = MakeSortedArray(5); // 0 2 4 6 8

            TestEqual(TEXT("exact match"), Rem::Algo::LowerBound(Numbers, 4), 2);
            TestEqual(TEXT("between values"), Rem::Algo::LowerBound(Numbers, 3), 2);
            TestEqual(TEXT("first element"), Rem::Algo::LowerBound(Numbers, 0), 0);
            TestEqual(TEXT("before all"), Rem::Algo::LowerBound(Numbers, -5), 0);
            TestEqual(TEXT("after all"), Rem::Algo::LowerBound(Numbers, 10), 5);
        });

        It(TEXT("should support a projection"), [this]
        {
            struct FPair
            {
                int32 Key;
            };
            TArray<FPair> Pairs{{FPair{0}}, {FPair{2}}, {FPair{4}}};

            TestEqual(TEXT("projected"), Rem::Algo::LowerBound(Pairs, 3,
                [](const FPair& Pair) { return Pair.Key; }), 2);
        });
    });

    Describe(TEXT("UpperBound"), [this]
    {
        It(TEXT("should find the first element greater than the value"), [this]
        {
            auto Numbers = MakeSortedArray(5); // 0 2 4 6 8

            TestEqual(TEXT("exact match"), Rem::Algo::UpperBound(Numbers, 4), 3);
            TestEqual(TEXT("between values"), Rem::Algo::UpperBound(Numbers, 3), 2);
            TestEqual(TEXT("before all"), Rem::Algo::UpperBound(Numbers, -5), 0);
            TestEqual(TEXT("after all"), Rem::Algo::UpperBound(Numbers, 10), 5);
        });
    });

    Describe(TEXT("BinarySearch"), [this]
    {
        It(TEXT("should return the index of the value when present"), [this]
        {
            auto Numbers = MakeSortedArray(5);

            TestEqual(TEXT("found"), Rem::Algo::BinarySearch(Numbers, 6), 3);
        });

        It(TEXT("should return INDEX_NONE when absent"), [this]
        {
            auto Numbers = MakeSortedArray(5);

            TestEqual(TEXT("not found"), Rem::Algo::BinarySearch(Numbers, 3), INDEX_NONE);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
