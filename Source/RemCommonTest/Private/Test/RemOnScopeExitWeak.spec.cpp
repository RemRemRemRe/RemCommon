// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemOnScopeExitWeak.h"

#include "Misc/AutomationTest.h"

#include "RemOnScopeExitWeakTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Common::Private::URemOnScopeExitWeakTestObject;

DEFINE_SPEC(FRemOnScopeExitWeakTest, "Rem.Common.OnScopeExitWeak",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemOnScopeExitWeakTest::Define()
{
    Describe(TEXT("REM_ON_SCOPE_EXIT_WEAK"), [this]
    {
        It(TEXT("should execute the lambda when the scope exits"), [this]
        {
            URemOnScopeExitWeakTestObject* Object = NewObject<URemOnScopeExitWeakTestObject>();

            TestFalse(TEXT("not yet"), Object->bExecuted);

            Object->Run();

            TestTrue(TEXT("executed on scope exit"), Object->bExecuted);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
