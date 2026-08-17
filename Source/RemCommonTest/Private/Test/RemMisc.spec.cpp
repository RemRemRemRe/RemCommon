// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemScopedStructContainerTestStructs.h"

#include "Curves/CurveFloat.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Misc/AutomationTest.h"
#include "RemMisc.h"
#include "RemNotNull.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Struct::Private::FRemScopedStructContainerTestTrivial;
}

DEFINE_SPEC(FRemMiscTest, "Rem.Common.Misc",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemMiscTest::Define()
{
    Describe(TEXT("IsValid"), [this]
    {
        It(TEXT("should validate raw pointers"), [this]
        {
            auto* Object = NewObject<UDataTable>();

            TestTrue(TEXT("valid pointer"), Rem::IsValid(Object));
            TestTrue(TEXT("null pointer"), !Rem::IsValid(static_cast<UObject*>(nullptr)));
        });

        It(TEXT("should validate TObjectPtr"), [this]
        {
            auto* Object = NewObject<UDataTable>();
            const TObjectPtr<UDataTable> ObjectPtr{Object};
            const TObjectPtr<UObject> NullPtr{};

            TestTrue(TEXT("valid object ptr"), Rem::IsValid(ObjectPtr));
            TestTrue(TEXT("null object ptr"), !Rem::IsValid(NullPtr));
        });

        It(TEXT("should validate TNotNull wrapping a valid object"), [this]
        {
            auto* Object = NewObject<UDataTable>();
            const Rem::TNotNull<UDataTable*> NotNull{Object};

            TestTrue(TEXT("not null is valid"), Rem::IsValid(NotNull));
        });

        It(TEXT("should validate values with an IsValid member"), [this]
        {
            const auto Tag = FGameplayTag::EmptyTag;

            TestTrue(TEXT("invalid gameplay tag"), !Rem::IsValid(Tag));
        });
    });

    Describe(TEXT("EvaluateCurve01"), [this]
    {
        It(TEXT("should map the alpha onto the curve time range"), [this]
        {
            auto* Curve = NewObject<UCurveFloat>();
            Curve->FloatCurve.AddKey(2.0f, 10.0f);
            Curve->FloatCurve.AddKey(4.0f, 30.0f);

            // alpha 0.5 -> time 3 -> value 20
            TestEqual(TEXT("mapped value"), Rem::EvaluateCurve01(Curve->FloatCurve, 0.5f), 20.0f, 1e-4f);
        });

        It(TEXT("EvaluateCurve01Clamped should clamp the value into [0, 1]"), [this]
        {
            auto* Curve = NewObject<UCurveFloat>();
            Curve->FloatCurve.AddKey(0.0f, 2.0f);
            Curve->FloatCurve.AddKey(1.0f, 2.0f);

            TestEqual(TEXT("clamped value"), Rem::EvaluateCurve01Clamped(Curve->FloatCurve, 0.5f), 1.0f, 1e-4f);
        });
    });

    Describe(TEXT("ToString"), [this]
    {
        It(TEXT("should render a bool"), [this]
        {
            TStringBuilder<32> Builder;
            Rem::ToString(Builder, true);

            TestEqual(TEXT("true"), FStringView{Builder.ToString()}, FStringView{TEXT("true")});
        });

    });

    Describe(TEXT("Memory helpers"), [this]
    {
        It(TEXT("AllocateStructMemory and FreeStructMemory should round trip a struct"), [this]
        {
            const auto* Struct = FRemScopedStructContainerTestTrivial::StaticStruct();
            auto& Memory = *Rem::AllocateStructMemory(*Struct);

            auto& Data = *reinterpret_cast<FRemScopedStructContainerTestTrivial*>(&Memory);
            Data.Number = 3.5f;
            Data.Count = 7;
            TestEqual(TEXT("number"), Data.Number, 3.5f, 1e-4f);
            TestEqual(TEXT("count"), Data.Count, 7);

            Rem::FreeStructMemory(*Struct, Memory);
        });
    });

    Describe(TEXT("Object checks"), [this]
    {
        It(TEXT("IsClassDefaultObject should distinguish CDOs from instances"), [this]
        {
            const auto* DefaultObject = GetDefault<UDataTable>();
            auto* Instance = NewObject<UDataTable>();

            TestTrue(TEXT("default object"), Rem::IsClassDefaultObject(DefaultObject));
            TestFalse(TEXT("instance"), Rem::IsClassDefaultObject(Instance));
        });

        It(TEXT("GetObjectNameFromSoftObjectPath should extract the object name"), [this]
        {
            const FSoftObjectPath Path{TEXT("/Game/Test/MyAsset.MyAsset.Outer.Inner")};

            TestEqual(TEXT("object name"), Rem::GetObjectNameFromSoftObjectPath(Path),
                FUtf8String{TEXT("Inner")});
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
