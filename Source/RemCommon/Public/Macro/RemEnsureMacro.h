// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemMacroUtilities.h"

// we may want faster "ensure" during development, but want the slower version that print c++ callstack and sends reports in other build
#define REM_USE_LIGHT_WEIGHT_ENSURE REM_WITH_DEVELOPMENT_ONLY_CODE

// The _MESSAGE variants of the two branches below take different format types: the lightweight branch takes a UTF8
// format literal, because it formats, logs and broadcasts the message as UTF8, while the heavy branch maps them to
// ensureMsgf/ensureAlwaysMsgf, which take a TCHAR format. A _MESSAGE call site written with a UTF8 literal therefore
// cannot compile when USE_ENSURES_IN_SHIPPING turns ensures on in a build where the lightweight branch is off, so
// such a call site has to be compiled out there; today exactly one exists, under
// #if !(UE_BUILD_SHIPPING || UE_BUILD_TEST).

#if REM_USE_LIGHT_WEIGHT_ENSURE

#include "Macro/RemAlsMacros.h"

#define REM_ASSERTION_INTERNAL_ENSURE(Condition) \
    { \
        constexpr bool RightConditionIsFalse{false}; \
        REM_ALS_ENSURE(RightConditionIsFalse && ( Condition )); \
    }

#define REM_ASSERTION_INTERNAL_ENSURE_ALWAYS(Condition) \
    { \
        constexpr bool RightConditionIsFalse{false}; \
        REM_ALS_ENSURE_ALWAYS(RightConditionIsFalse && ( Condition )); \
    }

#define REM_ENSURE(Expression) REM_ALS_ENSURE(Expression)
#define REM_ENSURE_MESSAGE(Expression, Format, ...) REM_ALS_ENSURE_MESSAGE(Expression, Format, ##__VA_ARGS__)
#define REM_ENSURE_ALWAYS(Expression) REM_ALS_ENSURE_ALWAYS(Expression)
#define REM_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) REM_ALS_ENSURE_ALWAYS_MESSAGE(Expression, Format, ##__VA_ARGS__)

#else

#include "Misc/AssertionMacros.h"

#define REM_ASSERTION_INTERNAL_ENSURE(Condition) \
    { \
        constexpr bool RightConditionIsFalse{false}; \
        ensure(RightConditionIsFalse && ( Condition )); \
    }

#define REM_ASSERTION_INTERNAL_ENSURE_ALWAYS(Condition) \
    { \
        constexpr bool RightConditionIsFalse{false}; \
        ensureAlways(RightConditionIsFalse && ( Condition )); \
    }

#define REM_ENSURE(Expression) ensure(Expression)
#define REM_ENSURE_MESSAGE(Expression, Format, ...) ensureMsgf(Expression, Format, ##__VA_ARGS__)
#define REM_ENSURE_ALWAYS(Expression) ensureAlways(Expression)
#define REM_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) ensureAlwaysMsgf(Expression, Format, ##__VA_ARGS__)

#endif
