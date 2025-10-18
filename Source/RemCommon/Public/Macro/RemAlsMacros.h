// Copyright Sixze, RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Misc/AssertionMacros.h"

#define REM_API REMCOMMON_API

// #define REM_GET_TYPE_STRING(Type) \
// 	((void) sizeof UEAsserts_Private::GetMemberNameCheckedJunk(static_cast<Type*>(nullptr)), TEXTVIEW(#Type))

// A lightweight version of the ensure() macro that doesn't generate a C++ call stack and doesn't send a
// crash report, because it doesn't happen instantly and causes the editor to freeze, which can be annoying.

#if DO_ENSURE && !USING_CODE_ANALYSIS

namespace Rem::AlsEnsure
{
	struct FAlsEnsureInfo
	{
		const ANSICHAR* Expression{nullptr};

		const ANSICHAR* FilePath{nullptr};

		int32 LineNumber{0};

		uint8 bEnsureAlways : 1 {false};
	};

	REM_API bool UE_COLD UE_DEBUG_SECTION VARARGS
	Execute(std::atomic<uint8>& bExecuted, const FAlsEnsureInfo& EnsureInfo);

	REM_API bool UE_COLD UE_DEBUG_SECTION VARARGS
	ExecuteFormat(std::atomic<uint8>& bExecuted, const FAlsEnsureInfo& EnsureInfo, const TCHAR* Format, ...);
}

#define REM_ALS_ENSURE_IMPLEMENTATION(bEnsureAlways, Expression) \
	(LIKELY(Expression) || \
	 (Rem::AlsEnsure::Execute(bGEnsureHasExecuted<FileLineHashForEnsure(__FILE__, __LINE__)>, \
	                    Rem::AlsEnsure::FAlsEnsureInfo{#Expression, __FILE__, __LINE__, bEnsureAlways}) && \
	  UE_BREAK_AND_RETURN_FALSE()))

#define REM_ALS_ENSURE_IMPLEMENTATION_FORMAT(bEnsureAlways, Expression, Format, ...) \
	(LIKELY(Expression) || [&]() UE_COLD UE_DEBUG_SECTION \
	{ \
		static constexpr Rem::AlsEnsure::FAlsEnsureInfo EnsureInfo{#Expression, __builtin_FILE(), __builtin_LINE(), bEnsureAlways}; \
 		\
		UE_VALIDATE_FORMAT_STRING(Format, ##__VA_ARGS__); \
		\
		if (Rem::AlsEnsure::ExecuteFormat(bGEnsureHasExecuted<FileLineHashForEnsure(__FILE__, __LINE__)>, EnsureInfo, Format, ##__VA_ARGS__)) \
		{ \
			PLATFORM_BREAK(); \
		} \
		\
		return false; \
	}())

#define REM_ALS_ENSURE(Expression) REM_ALS_ENSURE_IMPLEMENTATION(false, Expression)
#define REM_ALS_ENSURE_ALWAYS(Expression) REM_ALS_ENSURE_IMPLEMENTATION(true, Expression)
#define REM_ALS_ENSURE_MESSAGE(Expression, Format, ...) REM_ALS_ENSURE_IMPLEMENTATION_FORMAT(false, Expression, Format, ##__VA_ARGS__)
#define REM_ALS_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) REM_ALS_ENSURE_IMPLEMENTATION_FORMAT(true, Expression, Format, ##__VA_ARGS__)

#else

#define REM_ALS_ENSURE(Expression) (Expression)
#define REM_ALS_ENSURE_ALWAYS(Expression) (Expression)
#define REM_ALS_ENSURE_MESSAGE(Expression, Format, ...) (Expression)
#define REM_ALS_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...) (Expression)

#endif

#undef REM_API
