// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemMisc.h"

#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Test/RemScopedStructContainerTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Struct::Private::FRemScopedStructContainerTestTrivial;

DEFINE_SPEC(FRemMiscExtraTest, "Rem.Common.MiscExtra",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemMiscExtraTest::Define()
{
    Describe(TEXT("BoolToString"), [this]
    {
        It(TEXT("should produce the documented strings"), [this]
        {
            TestEqual(TEXT("true"), Rem::BoolToString(true), ANSITEXTVIEW("True"));
            TestEqual(TEXT("false"), Rem::BoolToString(false), ANSITEXTVIEW("False"));
        });
    });

    Describe(TEXT("IsClassDefaultObject"), [this]
    {
        It(TEXT("should recognize class default objects"), [this]
        {
            TestTrue(TEXT("cdo"),
                Rem::IsClassDefaultObject(GetDefault<AActor>()));
            TestFalse(TEXT("transient"),
                Rem::IsClassDefaultObject(NewObject<AActor>()));
        });
    });

    Describe(TEXT("GetObjectNameFromSoftObjectPath"), [this]
    {
        It(TEXT("should extract the innermost object name after the last dot"), [this]
        {
            const FSoftObjectPath Path{TEXT("/Game/Rem/Character/Ability/GameplayEffect/GE_Damage.GE_Damage:Inner.Child")};

            TestEqual(TEXT("name"), Rem::GetObjectNameFromSoftObjectPath(Path),
                FUtf8String{TEXTVIEW("Child")});
        });
    });

    Describe(TEXT("PointerToString"), [this]
    {
        It(TEXT("should format a pointer address"), [this]
        {
            const auto* Object = GetDefault<AActor>();

            const auto String = Rem::PointerToString(Rem::MakeNotNull(static_cast<const void*>(Object)));

            TestTrue(TEXT("non-empty"), !String.IsEmpty());
        });
    });

    Describe(TEXT("ToString with a script struct"), [this]
    {
        It(TEXT("should export the struct through its script struct"), [this]
        {
            FRemScopedStructContainerTestTrivial Value;
            Value.Count = 42;

            const auto String = Rem::ToString(Rem::MakeNotNull(
                static_cast<const UScriptStruct*>(FRemScopedStructContainerTestTrivial::StaticStruct())),
                Rem::MakeNotNull(static_cast<const void*>(&Value)));

            TestTrue(TEXT("contains the value"), String.Contains(UTF8TEXTVIEW("42")));
        });
    });

    Describe(TEXT("GetNetDebugString"), [this]
    {
        It(TEXT("should produce a net debug string for an object"), [this]
        {
            TUtf8StringBuilder<64> Builder;
            Rem::GetNetDebugString(Builder, GetDefault<AActor>());

            TestTrue(TEXT("non-empty"), !FUtf8String(Builder).IsEmpty());
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
