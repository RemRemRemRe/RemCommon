// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemClassWrapper.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemClassWrapperTest, "Rem.Common.ClassWrapper",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemClassWrapperTest::Define()
{
    Describe(TEXT("FRemClassWrapper"), [this]
    {
        It(TEXT("should default to no class"), [this]
        {
            constexpr FRemClassWrapper Wrapper{};

            TestTrue(TEXT("no class"), Wrapper.Class == nullptr);
            TestTrue(TEXT("no object"), Wrapper.GetObject() == nullptr);
        });

        It(TEXT("should expose the class through the getters"), [this]
        {
            FRemClassWrapper Wrapper;
            Wrapper.Class = AActor::StaticClass();

            TestTrue(TEXT("class getter"), Wrapper.GetClass() == AActor::StaticClass());
            TestTrue(TEXT("class template getter"),
                Wrapper.GetClass<AActor>() == AActor::StaticClass());
            TestTrue(TEXT("class T getter"), Wrapper.GetClassT<AActor>() == AActor::StaticClass());
            TestTrue(TEXT("object getter"), Wrapper.GetObject() == AActor::StaticClass());
            TestTrue(TEXT("object template getter"),
                Wrapper.GetObject<AActor>() == AActor::StaticClass());
        });

        It(TEXT("should compare by class"), [this]
        {
            FRemClassWrapper First;
            First.Class = AActor::StaticClass();

            FRemClassWrapper Second;
            Second.Class = AActor::StaticClass();

            FRemClassWrapper Different;
            Different.Class = APawn::StaticClass();

            TestTrue(TEXT("equal"), First == Second);
            TestFalse(TEXT("different"), First == Different);
        });
    });

    Describe(TEXT("FRemClassArrayWrapper"), [this]
    {
        It(TEXT("should expose the class array through the getters"), [this]
        {
            FRemClassArrayWrapper Wrapper;

            FRemClassWrapper& Entry = Wrapper.Classes.Emplace_GetRef();
            Entry.Class = AActor::StaticClass();

            TestEqual(TEXT("element count"), Wrapper.GetElements().Num(), 1);
            TestEqual(TEXT("count getter"), Wrapper.Get().Num(), 1);
            TestTrue(TEXT("entry class"), Wrapper.Get()[0].GetClass() == AActor::StaticClass());
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
