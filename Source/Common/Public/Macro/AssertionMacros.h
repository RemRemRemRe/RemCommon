//

#pragma once

#include "Misc.h"
//#include "CoreMinimal.h" // for FString and ensure
//#include "Delegates/DelegateInstancesImpl.h" // for UE4Delegates_Private::IsUObjectPtr
//#include "UObject/Object.h" // for FORCE INLINE bool IsValid(const UObject *Test)

#pragma region Helper Macro Definition

// @see https://stackoverflow.com/a/56038661 and related posts it referred
//  detailed explanation : https://stackoverflow.com/a/28074198

// This part you put in some library header:
#define FUNC_CHOOSER(_f0, _f1, _f2, _f3, _f4, _f5, _f6, _f7, _f8, _f9, _f10, _f11, _f12, _f13, _f14, _f15, _f16, ...) _f16
#define FUNC_COMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(F, ...) FUNC_COMPOSER((__VA_ARGS__, \
            F##_16, F##_15, F##_14, F##_13, F##_12, F##_11, F##_10, F##_9, F##_8,\
            F##_7, F##_6, F##_5, F##_4, F##_3, F##_2, F##_1, ))
#define NO_ARG_EXPANDER(FUNC) ,,,,,,,,,,,,,,,,FUNC ## _0
#define MACRO_CHOOSER(FUNC, ...) CHOOSE_FROM_ARG_COUNT(FUNC, NO_ARG_EXPANDER __VA_ARGS__ (FUNC))
#define MULTI_MACRO(FUNC, ...) MACRO_CHOOSER(FUNC, __VA_ARGS__)(__VA_ARGS__)

#pragma endregion Helper Macro Definition

#pragma region Config Macro

// #define LET_IT_CRASH
// #define SUPPRESS_LOG
// #define NO_BREAKPOINT
// #define NO_CHECK_CONDITION

#ifdef LET_IT_CRASH
    #define INVALID_HANDLING_STATEMENT(...)
#else
    #define INVALID_HANDLING_STATEMENT(...) __VA_ARGS__
#endif // LET_IT_CRASH


#ifdef SUPPRESS_LOG
    #define LOG_HELPER(CategoryName, Verbosity, Message)
#else
    #define LOG_HELPER(CategoryName, Verbosity, Message) \
        do \
        { \
            const FString MessageString{Message}; \
            if (!( true == MessageString.IsEmpty() )) \
            { \
                UE_LOG(CategoryName, Verbosity, TEXT("%s"), *MessageString) \
            } \
        } while (false)
#endif // SUPPRESS_LOG

namespace AssertionMacros
{
    enum class EBreakMode : uint8
    {
        NoBreak,
        BreakAtFirstHit,
        AlwaysBreak
    };
}

#ifdef NO_BREAKPOINT
    #define BREAK_POINTER(Condition, BreakMode)
#else
    #define BREAK_POINTER(Condition, BreakMode) \
        do \
        { \
            using namespace AssertionMacros; \
            constexpr EBreakMode Mode = BreakMode; \
            switch (Mode) \
            { \
            case EBreakMode::NoBreak: \
                break; \
            case EBreakMode::BreakAtFirstHit: \
                ensure(Condition); \
                break; \
            case EBreakMode::AlwaysBreak: \
                ensureAlways(Condition); \
                break; \
            default: \
                break; \
            } \
        } while (false)

#endif // NO_BREAKPOINT

#pragma endregion Config Macro

#pragma region Ensure Condition

#define ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, BreakMode, Message, CategoryName, Verbosity) \
    { \
        if ( UNLIKELY( !(Condition) ) ) \
        { \
            LOG_HELPER(CategoryName, Verbosity, Message); \
            { \
                constexpr bool RightConditionIsFalse = false; \
                BREAK_POINTER(RightConditionIsFalse && ( Condition ), BreakMode); \
            } \
            INVALID_HANDLING_STATEMENT(InvalidHandlingStatement); \
        } \
    }

#define ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, BreakMode, Message, CategoryName) \
    ENSURE_CONDITION_6(Condition, InvalidHandlingStatement, BreakMode, Message, CategoryName, Warning)

#define ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, BreakMode, Message) \
    ENSURE_CONDITION_5(Condition, InvalidHandlingStatement, BreakMode, Message, LogTemp)

#define ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, BreakMode) \
    ENSURE_CONDITION_4(Condition, InvalidHandlingStatement, BreakMode, FString()/*Message*/)

#define ENSURE_CONDITION_2(Condition, InvalidHandlingStatement) \
    ENSURE_CONDITION_3(Condition, InvalidHandlingStatement, EBreakMode::AlwaysBreak)

#define ENSURE_CONDITION_1(Condition) \
    ENSURE_CONDITION_2(Condition, /*InvalidHandlingStatement*/)

/**
 * Use this to validate a condition.
 *  - Condition is necessary, other parameters are optional.
 *
 * @param Condition                 the condition to validate
 * @param InvalidHandlingStatement  statements executed when the pointer is invalid. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param BreakMode                 break mode to use when pointer is invalid. default value is EBreakMode::AlwaysBreak
 * @param Message                   formatted log message when pointer is invalid
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity level to test against. default to ELogVerbosity::Type::Warning
 */
#define EnsureCondition(...) MULTI_MACRO(ENSURE_CONDITION, __VA_ARGS__)

#pragma endregion Ensure Condition

#pragma region Ensure Pointer

#define ENSURE_POINTER_6(Pointer, InvalidHandlingStatement, BreakMode, Message, CategoryName, Verbosity) \
    ENSURE_CONDITION_6(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, BreakMode, Message, CategoryName, Verbosity)

#define ENSURE_POINTER_5(Pointer, InvalidHandlingStatement, BreakMode, Message, CategoryName) \
    ENSURE_CONDITION_5(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, BreakMode, Message, CategoryName)

#define ENSURE_POINTER_4(Pointer, InvalidHandlingStatement, BreakMode, Message) \
    ENSURE_CONDITION_4(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, BreakMode, Message)

#define ENSURE_POINTER_3(Pointer, InvalidHandlingStatement, BreakMode) \
    ENSURE_CONDITION_3(VALIDATE_POINTER(Pointer), InvalidHandlingStatement, BreakMode)

#define ENSURE_POINTER_2(Pointer, InvalidHandlingStatement) \
    ENSURE_CONDITION_2(VALIDATE_POINTER(Pointer), InvalidHandlingStatement)

#define ENSURE_POINTER_1(Pointer) \
    ENSURE_CONDITION_1(VALIDATE_POINTER(Pointer))

/**
 * Use this to validate a pointer.
 *  - Pointer is necessary, other parameters are optional.
 *
 * @param Pointer                   the pointer to validate
 * @param InvalidHandlingStatement  statements executed when the pointer is invalid. can be any number of statements as you want (surround them with '{}' if ',' is used)
 * @param BreakMode                 break mode to use when pointer is invalid. default value is EBreakMode::AlwaysBreak
 * @param Message                   formatted log message when pointer is invalid
 * @param CategoryName              name of the logging category. default to LogTemp
 * @param Verbosity                 verbosity level to test against. default to ELogVerbosity::Type::Warning
 */
#define EnsurePointer(...) MULTI_MACRO(ENSURE_POINTER, __VA_ARGS__)

#pragma endregion Ensure Pointer

#pragma region Check Condition

#if defined(NO_CHECK_CONDITION) && !DO_CHECK
    #define CheckCondition(...)
    #define CheckPointer(...)
#else
    #define CheckCondition(...) EnsureCondition(__VA_ARGS__)
    #define CheckPointer(...) EnsurePointer(__VA_ARGS__)
#endif // NO_CHECK_CONDITION

#pragma endregion Check Condition
