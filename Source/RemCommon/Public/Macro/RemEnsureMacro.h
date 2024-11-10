// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemMacroUtilities.h"

// we may want faster "ensure" during development, but want the slower version that print c++ callstack and sends reports in other build
#if false && REM_WITH_DEVELOPMENT_ONLY_CODE

#include "Macro/RemFuMacros.h"

#define REM_ASSERTION_INTERNAL_ENSURE(Condition) REM_FU_ENSURE_EVALUATED_TO_FALSE(Condition)
#define REM_ASSERTION_INTERNAL_ENSURE_ALWAYS(Condition) REM_FU_ENSURE_ALWAYS_EVALUATED_TO_FALSE(Condition)

#define REM_ENSURE(Expression) REM_FU_ENSURE(Expression)
#define REM_ENSURE_MESSAGE(Expression, Format, ...) REM_FU_ENSURE(Expression, Format, ##__VA_ARGS__)
#define REM_ENSURE_ALWAYS(Expression) REM_FU_ENSURE_ALWAYS(Expression)
#define REM_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) REM_FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ##__VA_ARGS__)

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
