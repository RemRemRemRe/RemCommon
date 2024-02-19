// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemMacroAsFunctionOverloads.h"
#include "RemMisc.h"

#pragma region Config Macro

// #define REM_LET_IT_CRASH
// #define REM_DISABLE_LOG
// #define REM_DISABLE_ASSERTION
#define DISABLE_CHECK_MACRO !(DO_CHECK || REM_WITH_DEVELOPMENT_ONLY_CODE)

#ifdef REM_LET_IT_CRASH
    #define REM_INVALID_HANDLING_STATEMENT(...)
#else
    #define REM_INVALID_HANDLING_STATEMENT(...) __VA_ARGS__
#endif // REM_LET_IT_CRASH


#ifdef REM_DISABLE_LOG
    #define REM_LOG_HELPER(CategoryName, Verbosity, Message)
#else
    #define REM_LOG_HELPER(CategoryName, Verbosity, Message) \
        do \
        { \
            if constexpr ( constexpr std::string_view StringView = #Message; \
            !StringView.empty()) \
            { \
                UE_LOG(CategoryName, Verbosity, TEXT("Frame:%d, %s"), ::GFrameNumber, *FString(Message) ) \
            } \
        } while (false)
#endif // REM_DISABLE_LOG


#ifdef REM_DISABLE_ASSERTION
    #define REM_ASSER_CONDITION(AssertionMacro, Condition)
    #define REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition)
#else
    #define REM_ASSER_CONDITION(AssertionMacro, Condition) \
        do \
        { \
            if constexpr ( constexpr std::string_view StringView = #AssertionMacro; \
            !StringView.empty()) \
            { \
                AssertionMacro(Condition); \
            } \
        } while (false)

    #define REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition) \
        do \
        { \
            constexpr bool RightConditionIsFalse = false; \
            REM_ASSER_CONDITION(AssertionMacro, RightConditionIsFalse && ( Condition )); \
        } while (false)

#endif // REM_DISABLE_ASSERTION

#pragma region Dummy Macro For Readability

#define REM_NO_HANDLING

#define REM_NO_LOG

#define REM_NO_ASSERTION

#define REM_NO_LOG_OR_ASSERTION LogTemp, Log, REM_NO_LOG, REM_NO_ASSERTION

#define REM_NO_LOG_BUT_ENSURE LogTemp, Log, REM_NO_LOG, ensure

#pragma endregion Dummy Macro For Readability

#pragma endregion Config Macro

#pragma region Ensure Condition

#define REM_ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, CategoryName, Verbosity, Message, AssertionMacro) \
    { \
        if (!LIKELY(Condition)) \
        { \
            REM_LOG_HELPER(CategoryName, Verbosity, Message); \
            { \
                REM_ASSER_CONDITION_EVALUATED(AssertionMacro, Condition ); \
            } \
            REM_INVALID_HANDLING_STATEMENT(InvalidHandlingStatement); \
        } \
    }

#define REM_ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, CategoryName, Verbosity, Message) \
    REM_ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, CategoryName, Verbosity, Message, ensureAlways)

#define REM_ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, CategoryName, Verbosity) \
    REM_ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, CategoryName, Verbosity, /* Empty Message */)

#define REM_ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, CategoryName) \
    REM_ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, CategoryName, Log)

#define REM_ENSURE_CONDITION_2(Condition, InvalidHandlingStatement) \
    REM_ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, LogTemp)

#define REM_ENSURE_CONDITION_1(Condition) \
    REM_ENSURE_CONDITION_2(Condition, /*InvalidHandlingStatement*/)

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 * @param Message                   formatted log message when condition is false
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways
 */
#define RemEnsureCondition(...) REM_MULTI_MACRO(REM_ENSURE_CONDITION, ##__VA_ARGS__)

#pragma endregion Ensure Condition

#pragma region Ensure Variable

#define REM_ENSURE_VARIABLE_6(Pointer, InvalidHandlingStatement, CategoryName, Verbosity, Message, AssertionMacro) \
    REM_ENSURE_CONDITION_6(Rem::IsValid(Pointer), InvalidHandlingStatement, CategoryName, Verbosity, Message, AssertionMacro)

#define REM_ENSURE_VARIABLE_5(Pointer, InvalidHandlingStatement, CategoryName, Verbosity, Message) \
    REM_ENSURE_CONDITION_5(Rem::IsValid(Pointer), InvalidHandlingStatement, CategoryName, Verbosity, /* Empty Message */)

#define REM_ENSURE_VARIABLE_4(Pointer, InvalidHandlingStatement, CategoryName, Verbosity) \
    REM_ENSURE_CONDITION_4(Rem::IsValid(Pointer), InvalidHandlingStatement, CategoryName, Log)

#define REM_ENSURE_VARIABLE_3(Pointer, InvalidHandlingStatement, CategoryName) \
    REM_ENSURE_CONDITION_3(Rem::IsValid(Pointer), InvalidHandlingStatement, LogTemp)

#define REM_ENSURE_VARIABLE_2(Pointer, InvalidHandlingStatement) \
    REM_ENSURE_CONDITION_2(Rem::IsValid(Pointer), InvalidHandlingStatement)

#define REM_ENSURE_VARIABLE_1(Pointer) \
    REM_ENSURE_CONDITION_1(Rem::IsValid(Pointer))

/**
 * Use this to validate a variable.
 *  - variable is necessary, other parameters are optional.
 *
 * @param Variable					the variable to assert
 * @param InvalidHandlingStatement  statements to execute when pointer is invalid. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 * @param Message                   formatted log message when pointer is invalid
 * @param AssertionMacro            assertion macro to use when pointer is invalid. default to ensureAlways
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
 * @param Condition                 the condition to assert
 * @param InvalidHandlingStatement  statements to execute when condition is false. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 * @param Message                   formatted log message when condition is false
 * @param AssertionMacro            assertion macro to use when condition is false. default to ensureAlways
 */
#define RemCheckCondition(...) RemEnsureCondition(__VA_ARGS__)

/**
 * Use this to validate a variable.
 *  - Variable is necessary, other parameters are optional.
 *
 * @param Variable					the variable to assert
 * @param InvalidHandlingStatement  statements to execute when pointer is invalid. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity type. default to ELogVerbosity::Type::Warning
 * @param Message                   formatted log message when pointer is invalid
 * @param AssertionMacro            assertion macro to use when pointer is invalid. default to ensureAlways
 */
#define RemCheckVariable(...) RemEnsureVariable(__VA_ARGS__)

#endif // DISABLE_CHECK_MACRO

#pragma endregion Check Condition & Check Variable
