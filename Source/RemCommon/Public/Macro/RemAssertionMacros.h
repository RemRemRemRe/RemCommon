// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemMacroAsFunctionOverloads.h"
#include "RemMisc.h" // for Rem::IsValid
#include "RemEnsureMacro.h"

/**
 * Engine assertion macros, and how they differ from the REM_* family:
 * - check is fatal: a failed check aborts the process, and its whole expression is dropped when DO_CHECK == 0,
 *   which is the Test and Shipping configuration.
 * - ensure reports the failure and continues. When DO_ENSURE == 0 the expression is still evaluated but
 *   nothing is reported; otherwise a call site reports once per session.
 * - ensureAlways reports every failure, without the once-per-session limit.
 *
 * Prefer the REM_* macros. An engine assertion macro is only justified where the REM_* family deliberately
 * owns no such capability, for instance at a site that must abort.
 *
 * In a development build the REM_* macros report through Rem::AlsEnsure, the lightweight reporter declared in
 * RemAlsMacros.h: it logs the engine's message shape, generates no C++ call stack and submits no crash report.
 * The guard family's 1-argument and 2-argument forms default to REM_ENSURE_ALWAYS, so RemEnsureCondition /
 * RemEnsureVariable / RemCheckCondition / RemCheckVariable report through that path as well. In Shipping they map
 * to the engine's ensure/ensureAlways and the RemCheck* guards are stripped entirely; where ensures are compiled
 * out (DO_ENSURE == 0, the Test and Shipping defaults) nothing is reported.
 *
 * The lightweight reporter honours the engine's ensure switches:
 * - core.EnsureState is the once-per-call-site counter: a site reports while the counter differs from the value
 *   it last reported with, so changing the value re-arms every site that already reported (core.ResetEnsureState
 *   increments it).
 * - core.EnsureAlwaysEnabled set to false turns ensureAlways into ensure, i.e. at most one report per call site.
 * - core.EnsureBreakEnabled set to false stops a failing ensure from breaking into the debugger.
 * - core.EnsuresAreErrors decides whether the report is logged as an Error or as a Warning.
 */

#pragma region Config Macro

// #define REM_LET_IT_CRASH
// #define REM_DISABLE_ASSERTION

// RemCheck* is the development-only guard family: its guard is stripped where it cannot help, i.e. in
// Shipping. Use RemEnsure* for a guard that has to survive every configuration, and wrap a purely
// diagnostic one in REM_WITH_DEVELOPMENT_ONLY_CODE when it is unnecessary in development as well.
#define DISABLE_CHECK_MACRO UE_BUILD_SHIPPING

#ifdef REM_LET_IT_CRASH

#define REM_INVALID_HANDLING_STATEMENT(...)

#else

#define REM_INVALID_HANDLING_STATEMENT(...) __VA_ARGS__

#endif // REM_LET_IT_CRASH


#ifdef REM_DISABLE_ASSERTION

#define REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition)

#else

#define REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition) \
        do \
        { \
            if constexpr ( constexpr auto StringView = TEXTVIEW(#AssertionMacro); \
            !StringView.IsEmpty()) \
            { \
                constexpr bool ItsFalse{}; \
                AssertionMacro(ItsFalse && Condition); \
            } \
        } while (false)


#endif // REM_DISABLE_ASSERTION

#pragma region Dummy Macro For Readability

#define REM_NO_ASSERTION

#define REM_NO_HANDLING

#define REM_NO_ASSERTION_OR_HANDLING(Condition) REM_NO_ASSERTION, Condition, REM_NO_HANDLING

#pragma endregion Dummy Macro For Readability

#pragma endregion Config Macro

#pragma region Ensure Condition

#define REM_ENSURE_CONDITION_3(AssertionMacro, Condition, InvalidHandlingStatement) \
    { \
        if (!LIKELY(Condition)) \
        { \
            REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition ); \
            \
            REM_INVALID_HANDLING_STATEMENT(InvalidHandlingStatement); \
        } \
    }

#define REM_ENSURE_CONDITION_2(Condition, InvalidHandlingStatement) \
    REM_ENSURE_CONDITION_3(REM_ENSURE_ALWAYS, Condition, InvalidHandlingStatement)

#define REM_ENSURE_CONDITION_1(Condition) \
    REM_ENSURE_CONDITION_2(Condition, /* no handling */)

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. defaults to REM_ENSURE_ALWAYS
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemEnsureCondition(...) REM_MULTI_MACRO(REM_ENSURE_CONDITION, ##__VA_ARGS__)

#pragma endregion Ensure Condition

#pragma region Ensure Variable

#define REM_ENSURE_VARIABLE_3(AssertionMacro, Pointer, InvalidHandlingStatement) \
    REM_ENSURE_CONDITION_3(AssertionMacro, Rem::IsValid(Pointer), InvalidHandlingStatement)

#define REM_ENSURE_VARIABLE_2(Pointer, InvalidHandlingStatement) \
    REM_ENSURE_CONDITION_2(Rem::IsValid(Pointer), InvalidHandlingStatement)

#define REM_ENSURE_VARIABLE_1(Pointer) \
    REM_ENSURE_CONDITION_1(Rem::IsValid(Pointer))

/**
 * Use this to validate a variable.
 *  - variable is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. defaults to REM_ENSURE_ALWAYS
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemEnsureVariable(...) REM_MULTI_MACRO(REM_ENSURE_VARIABLE, ##__VA_ARGS__)

#pragma endregion Ensure Variable

#pragma region Check Condition & Check Variable

#if DISABLE_CHECK_MACRO

#define RemCheckCondition(...)
#define RemCheckVariable(...)

#else

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. defaults to REM_ENSURE_ALWAYS
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemCheckCondition(...) RemEnsureCondition(__VA_ARGS__)

/**
 * Use this to validate a variable.
 *  - variable is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. defaults to REM_ENSURE_ALWAYS
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemCheckVariable(...) RemEnsureVariable(__VA_ARGS__)

#endif // DISABLE_CHECK_MACRO

#pragma endregion Check Condition & Check Variable
