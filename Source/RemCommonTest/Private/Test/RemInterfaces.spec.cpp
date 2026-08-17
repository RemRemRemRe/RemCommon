// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Interface/RemInterfaceStatics.h"
#include "Interface/RemScriptStructInterface.h"
#include "Interface/RemStringInterface.h"
#include "Object/RemEmptyStruct.h"

#include "Misc/AutomationTest.h"

#include "RemInterfacesTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Common::Private::URemTestScriptStructObject;
using Rem::Common::Private::URemTestStringObject;

DEFINE_SPEC(FRemInterfacesTest, "Rem.Common.Interfaces",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemInterfacesTest::Define()
{
    Describe(TEXT("URemInterfaceStatics::IsValidInterface"), [this]
    {
        It(TEXT("should accept an interface bound to a valid object"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            FScriptInterface Interface;
            Interface.SetObject(Object);

            TestTrue(TEXT("valid"), URemInterfaceStatics::IsValidInterface(Interface));
        });

        It(TEXT("should reject an interface without an object"), [this]
        {
            FScriptInterface EmptyInterface;

            TestFalse(TEXT("invalid"), URemInterfaceStatics::IsValidInterface(EmptyInterface));
        });
    });

    Describe(TEXT("IRemScriptStructInterface"), [this]
    {
        It(TEXT("implementer should report its script struct"), [this]
        {
            URemTestScriptStructObject* Object = NewObject<URemTestScriptStructObject>();

            const IRemScriptStructInterface* Interface = Object;
            TestTrue(TEXT("script struct"),
                Interface->GetScriptStruct() == static_cast<const UScriptStruct*>(FRemEmptyStruct::StaticStruct()));
        });
    });

    Describe(TEXT("IRemStringInterface"), [this]
    {
        It(TEXT("implementer should return its string"), [this]
        {
            URemTestStringObject* Object = NewObject<URemTestStringObject>();

            const IRemStringInterface* Interface = Object;
            TestEqual(TEXT("string"), Interface->ToString(), FString{TEXT("RemTestStringObject")});
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
