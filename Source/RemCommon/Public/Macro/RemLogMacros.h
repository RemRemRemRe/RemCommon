// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

// for Rem::GetNetDebugString, Rem::StringFormat
#include "RemMisc.h"

// for REM_FUNCTION_NAME
#include "RemFunctionNameUtility.h"

// Extended log macro

#if NO_LOGGING

#define REM_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...)

#define REM_LOG_FUNCTION(CategoryName, Verbosity, Format, ...)

#define REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...)


#define REM_LOG_ROLE_COND(Condition, Object, CategoryName, Verbosity, Format, ...)

#define REM_LOG_FUNCTION_COND(Condition, CategoryName, Verbosity, Format, ...)

#define REM_LOG_ROLE_FUNCTION_COND(Condition, Object, CategoryName, Verbosity, Format, ...)


#define REM_LOG_ROLE_CVAR(ConsoleVariableName, Object, CategoryName, Verbosity, Format, ...)

#define REM_LOG_FUNCTION_CVAR(ConsoleVariableName, CategoryName, Verbosity, Format, ...)

#define REM_LOG_ROLE_FUNCTION_CVAR(ConsoleVariableName, Object, CategoryName, Verbosity, Format, ...)

#define REM_SCOPED_LOG(Object, CategoryName, Verbosity, LogStart, LogEnd)

#else

// Log message with optional role name prefix
#define REM_LOG_ROLE(Object, CategoryName, Verbosity, FormatString, ...) \
	do \
	{ \
        TUtf8StringBuilder<256> Builder; \
        \
		Rem::GetNetDebugString(Builder, Object); \
		Builder.AppendChar(' '); \
		\
        Rem::Format(Builder, FormatString, ##__VA_ARGS__); \
		\
        UE_LOGF(CategoryName, Verbosity, "%hs", *Builder); \
	} while (false)

// Log message with __FUNCTION__ suffix
#define REM_LOG_FUNCTION(CategoryName, Verbosity, FormatString, ...) \
	do \
	{ \
        TUtf8StringBuilder<256> Builder; \
        \
        Rem::Format(Builder, FormatString, ##__VA_ARGS__); \
		Builder.AppendChar(' '); \
		\
		Builder.Append(REM_FUNCTION_LINE_FORMATED); \
		\
        UE_LOGF(CategoryName, Verbosity, "%hs", *Builder); \
	} while (false)

// Log message with optional role name prefix, __FUNCTION__ suffix
#define REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, FormatString, ...) \
	do \
	{ \
        TUtf8StringBuilder<256> Builder; \
        \
        Rem::GetNetDebugString(Builder, Object); \
        Builder.AppendChar(' '); \
        \
        Rem::Format(Builder, FormatString, ##__VA_ARGS__); \
        Builder.AppendChar(' '); \
        \
        Builder.Append(REM_FUNCTION_LINE_FORMATED); \
        \
        UE_LOGF(CategoryName, Verbosity, "%hs", *Builder); \
	} while (false)


#define REM_LOG_ROLE_COND(Condition, Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (UNLIKELY(Condition)) \
		{ \
			REM_LOG_ROLE(Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#define REM_LOG_FUNCTION_COND(Condition, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (UNLIKELY(Condition)) \
		{ \
			REM_LOG_FUNCTION(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#define REM_LOG_ROLE_FUNCTION_COND(Condition, Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (UNLIKELY(Condition)) \
		{ \
			REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)


#define REM_LOG_ROLE_CVAR(ConsoleVariableName, Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (static const auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			REM_ENSURE(CVar)) \
		{ \
			REM_LOG_ROLE_COND(CVar->GetBool(), Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#define REM_LOG_FUNCTION_CVAR(ConsoleVariableName, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (static const auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			REM_ENSURE(CVar)) \
		{ \
			REM_LOG_FUNCTION_COND(CVar->GetBool(), CategoryName, Verbosity, Format, ##__VA_ARGS__) \
		} \
	} while (false)

#define REM_LOG_ROLE_FUNCTION_CVAR(ConsoleVariableName, Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (static const auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			REM_ENSURE(CVar)) \
		{ \
			REM_LOG_ROLE_FUNCTION_COND(CVar->GetBool(), Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#define REM_SCOPED_LOG(Object, CategoryName, Verbosity, LogStart, LogEnd) \
	ON_SCOPE_EXIT \
	{ \
		REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, LogStart); \
	}; \
	REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, LogEnd);

#endif
