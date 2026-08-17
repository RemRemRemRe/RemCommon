// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemIteratePropertiesOfTypeTestStructs.h"

#include "Misc/AutomationTest.h"
#include "UObject/UnrealType.h"
#include "RemNotNull.h"
#include "Templates/RemIteratePropertiesOfType.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Property::Private::FRemIterateTestData;
using Rem::Property::Private::FRemIterateTestNested;
using Rem::Property::Private::PopulateTestData;
using Rem::Property::IteratePropertiesOfType;

int32 CountVisits(const FRemIterateTestData& Data, const bool bConst)
{
    int32 Count{};
    const auto Functor = [&Count](const FProperty*, const void*)
    {
        ++Count;
    };

    if (bConst)
    {
        const Rem::TNotNull<const FRemIterateTestData*> NotNullData{&Data};
        IteratePropertiesOfType<FProperty>(FRemIterateTestData::StaticStruct(), NotNullData, Functor);
    }
    else
    {
        auto& MutableData = const_cast<FRemIterateTestData&>(Data);
        const Rem::TNotNull<FRemIterateTestData*> NotNullData{&MutableData};
        IteratePropertiesOfType<FProperty>(FRemIterateTestData::StaticStruct(), NotNullData, Functor);
    }

    return Count;
}
}

DEFINE_SPEC(FRemIteratePropertiesOfTypeTest, "Rem.Common.Property.IteratePropertiesOfType",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemIteratePropertiesOfTypeTest::Define()
{
    Describe(TEXT("FProperty mode"), [this]
    {
        It(TEXT("should visit every property including containers and their elements"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count](const FProperty*, void*)
                {
                    ++Count;
                });

            // 1 Number + 3 Nested + 4 Numbers + 7 NestedArray + 5 Map + 3 Set + 3 Instanced + 3 StaticArray
            TestEqual(TEXT("visits"), Count, 29);
        });

        It(TEXT("should visit a default instance without crashing"), [this]
        {
            FRemIterateTestData Data{};

            TestEqual(TEXT("visits"), CountVisits(Data, false), 12);
        });
    });

    Describe(TEXT("Typed mode"), [this]
    {
        It(TEXT("FIntProperty should visit every int member across all containers"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};
            int32 Sum{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FIntProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count, &Sum](const FIntProperty*, void* Value)
                {
                    ++Count;
                    Sum += *static_cast<int32*>(Value);
                });

            // 1 Number + 1 Nested + 3 Numbers + 2 NestedArray + 2 Map keys + 2 Set + 1 Instanced + 3 StaticArray
            TestEqual(TEXT("visits"), Count, 15);
            TestEqual(TEXT("value sum"), Sum, 141);
        });

        It(TEXT("FStrProperty should visit strings nested in structs and map values"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FStrProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count](const FStrProperty*, void*)
                {
                    ++Count;
                });

            // Nested text + 2 NestedArray texts + 2 Map values + instanced NestedText
            TestEqual(TEXT("visits"), Count, 6);
        });

        It(TEXT("FStructProperty should visit struct properties including array elements"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FStructProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count](const FStructProperty*, void*)
                {
                    ++Count;
                });

            // Nested + 2 NestedArray elements + Instanced
            TestEqual(TEXT("visits"), Count, 4);
        });
    });

    Describe(TEXT("Container modes"), [this]
    {
        It(TEXT("FArrayProperty should visit the array containers only"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FArrayProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count](const FArrayProperty*, void*)
                {
                    ++Count;
                });

            TestEqual(TEXT("visits"), Count, 2);
        });

        It(TEXT("FMapProperty and FSetProperty should visit their containers"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            auto MapCount = 0;
            auto SetCount = 0;

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FMapProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&MapCount](const FMapProperty*, void*)
                {
                    ++MapCount;
                });
            IteratePropertiesOfType<FSetProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&SetCount](const FSetProperty*, void*)
                {
                    ++SetCount;
                });

            TestEqual(TEXT("map visits"), MapCount, 1);
            TestEqual(TEXT("set visits"), SetCount, 1);
        });
    });

    Describe(TEXT("Instanced struct redirection"), [this]
    {
        It(TEXT("should descend into the instanced struct members"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);
            int32 Count{};
            int32 Sum{};

            const Rem::TNotNull<FRemIterateTestData*> NotNullData{&Data};
            IteratePropertiesOfType<FIntProperty>(FRemIterateTestData::StaticStruct(), NotNullData,
                [&Count, &Sum](const FIntProperty*, void* Value)
                {
                    ++Count;
                    Sum += *static_cast<int32*>(Value);
                });

            // the instanced NestedValue (9) is included in the totals
            TestEqual(TEXT("visits"), Count, 15);
            TestEqual(TEXT("sum includes instanced value"), Sum, 141);
        });
    });

    Describe(TEXT("Const path"), [this]
    {
        It(TEXT("should instantiate with a const container"), [this]
        {
            FRemIterateTestData Data{};
            PopulateTestData(Data);

            TestEqual(TEXT("visits"), CountVisits(Data, true), 29);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
