// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Struct/RemReflectedFunctionCallData.h"
#include "Struct/RemReflectedFunctionData.h"

#include "Misc/AutomationTest.h"

#include "Test/RemReflectedFunctionTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Common::Private::URemReflectedFunctionTestTarget;

DEFINE_SPEC(FRemReflectedFunctionTest, "Rem.Common.ReflectedFunction",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemReflectedFunctionTest::Define()
{
    Describe(TEXT("FRemReflectedFunctionData::GetFunction"), [this]
    {
        It(TEXT("should resolve a declared function of the owner class"), [this]
        {
            FRemReflectedFunctionData Data;
            Data.FunctionOwnerClass = URemReflectedFunctionTestTarget::StaticClass();
            Data.FunctionName = FName{TEXTVIEW("SetLastValue")};

            TestNotNull(TEXT("function"), Data.GetFunction());
        });

        It(TEXT("should return null without an owner or a name"), [this]
        {
            FRemReflectedFunctionData Empty;
            TestNull(TEXT("empty"), Empty.GetFunction());

            FRemReflectedFunctionData NoName;
            NoName.FunctionOwnerClass = URemReflectedFunctionTestTarget::StaticClass();
            TestNull(TEXT("no name"), NoName.GetFunction());
        });
    });

    Describe(TEXT("FRemReflectedFunctionCallData::TryFillParameters"), [this]
    {
        It(TEXT("should fail for missing function data"), [this]
        {
            FRemReflectedFunctionCallData CallData;

            TestFalse(TEXT("no function"), CallData.TryFillParameters());
        });

        It(TEXT("should fill the parameter bag from the function signature"), [this]
        {
            FRemReflectedFunctionCallData CallData;
            CallData.FunctionData.FunctionOwnerClass = URemReflectedFunctionTestTarget::StaticClass();
            CallData.FunctionData.FunctionName = FName{TEXTVIEW("SetLastValue")};

            TestTrue(TEXT("filled"), CallData.TryFillParameters());

            const auto* Desc = CallData.Parameters.FindPropertyDescByName(FName{TEXTVIEW("Value")});
            TestNotNull(TEXT("parameter desc"), Desc);
        });
    });

    Describe(TEXT("FRemReflectedFunctionCallData::Execute"), [this]
    {
        It(TEXT("should call a member function on the context object"), [this]
        {
            URemReflectedFunctionTestTarget* Target = NewObject<URemReflectedFunctionTestTarget>();

            FRemReflectedFunctionCallData CallData;
            CallData.FunctionData.FunctionOwnerClass = URemReflectedFunctionTestTarget::StaticClass();
            CallData.FunctionData.FunctionName = FName{TEXTVIEW("SetLastValue")};
            CallData.ContextObject = Target;

            TestTrue(TEXT("parameters filled"), CallData.TryFillParameters());
            CallData.Execute();

            TestEqual(TEXT("called with default parameters"), Target->LastValue, 0);
        });

        It(TEXT("should call a static function without a context object"), [this]
        {
            FRemReflectedFunctionCallData CallData;
            CallData.FunctionData.FunctionOwnerClass = URemReflectedFunctionTestTarget::StaticClass();
            CallData.FunctionData.FunctionName = FName{TEXTVIEW("GetStaticAnswer")};

            TestTrue(TEXT("parameters filled"), CallData.TryFillParameters());
            CallData.Execute();
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
