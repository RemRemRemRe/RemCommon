// Copyright Sixze, RemRemRemRe. 2024 All Rights Reserved.

#pragma once

#include "Misc/AssertionMacros.h"

#define REM_FU_STRINGIFY_IMPLEMENTATION(Value) #Value

#define REM_FU_STRINGIFY(Value) REM_FU_STRINGIFY_IMPLEMENTATION(Value)

#define REM_FU_GET_TYPE_STRING(Type) \
	((void) sizeof UEAsserts_Private::GetMemberNameCheckedJunk(static_cast<Type*>(nullptr)), TEXTVIEW(#Type))

// A lightweight version of the ensure() macro that doesn't generate a C++ call stack and doesn't send a
// crash report, because it doesn't happen instantly and causes the editor to freeze, which can be annoying.

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace Rem::FuEnsure
{
	REMCOMMON_API bool UE_DEBUG_SECTION VARARGS Execute(std::atomic<bool>& bExecuted, bool bEnsureAlways, const ANSICHAR* Expression,
	                                                          const TCHAR* StaticMessage, const TCHAR* Format, ...);
}

#define REM_FU_ENSURE_IMPLEMENTATION(Capture, bEnsureAlways, Expression, NoneTextAndExpression, Format, ...) \
	((LIKELY(NoneTextAndExpression) && LIKELY(Expression)) || [Capture]() UE_DEBUG_SECTION \
	{ \
		static constexpr auto StaticMessage{TEXT("Ensure failed: " #Expression ", File: " __FILE__ ", Line: " REM_FU_STRINGIFY(__LINE__) ".")}; \
 		static std::atomic<bool> bExecuted{false}; \
 		\
		UE_VALIDATE_FORMAT_STRING(Format, ##__VA_ARGS__); \
		\
		if (Rem::FuEnsure::Execute(bExecuted, bEnsureAlways, #Expression, StaticMessage, Format, ##__VA_ARGS__)) \
		{ \
			PLATFORM_BREAK(); \
		} \
		\
		return false; \
	}())

#define REM_FU_ENSURE(Expression) REM_FU_ENSURE_IMPLEMENTATION( , false, Expression, true, TEXT(""))
#define REM_FU_ENSURE_EVALUATED_TO_FALSE(Expression) REM_FU_ENSURE_IMPLEMENTATION( , false, Expression, false, TEXT(""))
#define REM_FU_ENSURE_MESSAGE(Expression, Format, ...) REM_FU_ENSURE_IMPLEMENTATION(&, false, Expression, true, Format, ##__VA_ARGS__)
#define REM_FU_ENSURE_ALWAYS(Expression) REM_FU_ENSURE_IMPLEMENTATION( , true, Expression, true, TEXT(""))
#define REM_FU_ENSURE_ALWAYS_EVALUATED_TO_FALSE(Expression) REM_FU_ENSURE_IMPLEMENTATION( , true, Expression, false, TEXT(""))
#define REM_FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) REM_FU_ENSURE_IMPLEMENTATION(&, true, Expression, true, Format, ##__VA_ARGS__)

#else

#define REM_FU_ENSURE(Expression) (Expression)
#define REM_FU_ENSURE_MESSAGE(Expression, Format, ...) (Expression)
#define REM_FU_ENSURE_ALWAYS(Expression) (Expression)
#define REM_FU_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) (Expression)

#endif
