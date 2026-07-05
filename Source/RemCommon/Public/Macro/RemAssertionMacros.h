// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemMacroAsFunctionOverloads.h"
#include "RemMisc.h" // for Rem::IsValid
#include "RemEnsureMacro.h"

#pragma region Config Macro

// #define REM_LET_IT_CRASH
// #define REM_DISABLE_ASSERTION
#define DISABLE_CHECK_MACRO false

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
    REM_ENSURE_CONDITION_3(ensureAlways, Condition, InvalidHandlingStatement)

#define REM_ENSURE_CONDITION_1(Condition) \
    REM_ENSURE_CONDITION_2(Condition, /* no handling */)

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways (or "similarities")
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
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways (or "similarities")
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
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways (or "similarities")
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemCheckCondition(...) RemEnsureCondition(__VA_ARGS__)

/**
 * Use this to validate a variable.
 *  - variable is necessary, other parameters are optional.
 *
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways (or "similarities")
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 */
#define RemCheckVariable(...) RemEnsureVariable(__VA_ARGS__)

#endif // DISABLE_CHECK_MACRO

#pragma endregion Check Condition & Check Variable
