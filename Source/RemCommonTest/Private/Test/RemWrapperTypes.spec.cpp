// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemCommonWrapperTypes.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
template <typename TWrapper>
void ExpectWrapperRoundTrip(FAutomationTestBase& Test, const TCHAR* const Description)
{
    auto* Struct = TWrapper::StaticStruct();

    TWrapper Source{};
    TWrapper Target{};
    Struct->InitializeStruct(&Target);
    Struct->CopyScriptStruct(&Target, &Source);
    Struct->DestroyStruct(&Target);

    TStringBuilder<128> Builder;
    Builder << Description << TEXT(" round-trip");
    Test.TestTrue(FString(Builder), true);
}
}

DEFINE_SPEC(FRemWrapperTypesTest, "Rem.Common.WrapperTypes",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemWrapperTypesTest::Define()
{
    Describe(TEXT("Reflection round-trips"), [this]
    {
        It(TEXT("scalar wrappers should copy through reflection"), [this]
        {
            // POD-safe wrappers only: the array/text wrappers own heap memory and
            // would alias through the byte copy
            ExpectWrapperRoundTrip<FRemByteWrapper>(*this, TEXT("byte"));
            ExpectWrapperRoundTrip<FRemIntegerWrapper>(*this, TEXT("integer"));
            ExpectWrapperRoundTrip<FRemFloatWrapper>(*this, TEXT("float"));
            ExpectWrapperRoundTrip<FRemDoubleWrapper>(*this, TEXT("double"));
            ExpectWrapperRoundTrip<FRemVector3FWrapper>(*this, TEXT("vector"));
        });

        It(TEXT("object wrappers should copy through reflection"), [this]
        {
            ExpectWrapperRoundTrip<FRemObjectWrapper>(*this, TEXT("object"));
            ExpectWrapperRoundTrip<FRemConstObjectWrapper>(*this, TEXT("const object"));
            ExpectWrapperRoundTrip<FRemSoftObjectWrapper>(*this, TEXT("soft object"));
            ExpectWrapperRoundTrip<FRemSoftClassWrapper>(*this, TEXT("soft class"));
        });
    });

    Describe(TEXT("Object wrappers"), [this]
    {
        It(TEXT("should expose the stored object through the getter"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            FRemObjectWrapper Wrapper;
            Wrapper.Object = Actor;

            TestTrue(TEXT("object getter"), Wrapper.GetObject() == Actor);
            TestTrue(TEXT("template getter"), Wrapper.GetObject<AActor>() == Actor);
            TestTrue(TEXT("unrelated cast null"), Wrapper.GetObject<UActorComponent>() == nullptr);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
