// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Macro/RemFloatDoubleConversionMacros.h"
#include "Macro/RemFunctionNameUtility.h"
#include "Macro/RemMacroUtilities.h"
#include "Macro/RemPrivateMemberAccessor.h"

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
// --- private member accessor setup -----------------------------------------

struct FRemMacroTestTarget
{
private:
    int32 SecretValue{7};
};

REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(FRemMacroTestAccessor,
    &FRemMacroTestTarget::SecretValue,
    int32 FRemMacroTestTarget::*);

// --- argument count dispatch -----------------------------------------------

#define REM_MACRO_TEST_FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define REM_MACRO_TEST_FUNC_COMPOSER(argsWithParentheses) REM_MACRO_TEST_FUNC_CHOOSER argsWithParentheses
#define REM_MACRO_TEST_CHOOSE_FROM_ARG_COUNT(F, ...) REM_MACRO_TEST_FUNC_COMPOSER((__VA_ARGS__, \
    F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8, F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, F##_0))
#define REM_MACRO_TEST_NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC##_0
#define REM_MACRO_TEST_MACRO_CHOOSER(FUNC, ...) REM_MACRO_TEST_CHOOSE_FROM_ARG_COUNT(FUNC, REM_MACRO_TEST_NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define REM_MACRO_TEST_MULTI_MACRO(FUNC, ...) REM_MACRO_TEST_MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)

#define REM_MACRO_TEST_FUNC_0() 0
#define REM_MACRO_TEST_FUNC_1(A1) A1
#define REM_MACRO_TEST_FUNC_2(A1, A2) A1 + A2
#define REM_MACRO_TEST_FUNC_3(A1, A2, A3) A1 + A2 + A3
}

DEFINE_SPEC(FRemMacrosTest, "Rem.Common.Macros",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemMacrosTest::Define()
{
    Describe(TEXT("REM_IF_THEN family"), [this]
    {
        It(TEXT("should apply the statement only when the condition holds"), [this]
        {
            auto Value = 1;

            REM_IF_THEN(Value == 1, Value = 2;);

            TestEqual(TEXT("applied"), Value, 2);

            REM_IF_THEN(Value == 99, Value = 3;);

            TestEqual(TEXT("skipped"), Value, 2);
        });
    });

    Describe(TEXT("REM_GET_TYPE_STRING"), [this]
    {
        It(TEXT("should produce the type name"), [this]
        {
            const auto TypeString = REM_GET_TYPE_STRING(AActor);

            TestEqual(TEXT("type name"), TypeString, ANSITEXTVIEW("AActor"));
        });
    });

    Describe(TEXT("REM_FUNCTION_NAME"), [this]
    {
        It(TEXT("should produce the current function name"), [this]
        {
            const auto Name = FString{REM_FUNCTION_NAME};

            TestTrue(TEXT("contains Define"), Name.Contains(TEXT("Define")));
        });
    });

    Describe(TEXT("Float/double conversion macros"), [this]
    {
        It(TEXT("should construct the double variants"), [this]
        {
            const auto Vector = REM_VECTOR3_DOUBLE(1, 2, 3);
            const auto Rotator = REM_ROTATOR_DOUBLE(0, 90, 0);
            const auto Quat = REM_QUAT_DOUBLE(0, 0, 0, 1);

            TestEqual(TEXT("vector x"), Vector.X, 1.0);
            TestEqual(TEXT("vector y"), Vector.Y, 2.0);
            TestEqual(TEXT("rotator yaw"), Rotator.Yaw, 90.0);
            TestEqual(TEXT("quat w"), Quat.W, 1.0);
        });

        It(TEXT("should construct the float variants"), [this]
        {
            const auto Vector = REM_VECTOR3_FLOAT(1, 2, 3);

            TestEqual(TEXT("vector z"), Vector.Z, 3.0f);
        });
    });

    Describe(TEXT("REM_DEFINE_PRIVATE_MEMBER_ACCESSOR"), [this]
    {
        It(TEXT("should access a private member through the accessor"), [this]
        {
            FRemMacroTestTarget Target;

            const auto Value = FRemMacroTestAccessor::Access(Target);

            TestEqual(TEXT("private value"), Value, 7);
        });
    });

    Describe(TEXT("Argument count dispatch"), [this]
    {
        It(TEXT("should dispatch by the number of arguments"), [this]
        {
            TestEqual(TEXT("zero args"), REM_MACRO_TEST_MULTI_MACRO(REM_MACRO_TEST_FUNC), 0);
            TestEqual(TEXT("one arg"), REM_MACRO_TEST_MULTI_MACRO(REM_MACRO_TEST_FUNC, 1), 1);
            TestEqual(TEXT("two args"), REM_MACRO_TEST_MULTI_MACRO(REM_MACRO_TEST_FUNC, 1, 2), 3);
            TestEqual(TEXT("three args"), REM_MACRO_TEST_MULTI_MACRO(REM_MACRO_TEST_FUNC, 1, 2, 3), 6);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
