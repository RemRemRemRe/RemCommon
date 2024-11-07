// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemMacroUtilities.h"
#include "RemMisc.h"

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

#else

#define REM_LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, FormatString, ...) \
	{ \
		const auto OriginalStr = Rem::StringFormat(FormatString, ##__VA_ARGS__); \
		const auto FinalStr = FString::Format(TEXT("{0} {1} {2}"), {std::move(Prefix), std::move(OriginalStr), std::move(Suffix)}); \
		UE_LOG(CategoryName, Verbosity, TEXT("Frame:%d %s"), ::GFrameNumber, *FinalStr); \
	}

// Log message with optional role name prefix
#define REM_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		REM_LOGGER_INTERNAL(Prefix, TEXT(""), CategoryName, Verbosity, Format, ##__VA_ARGS__) \
	} while (false)

// Log message with __FUNCTION__ suffix
#define REM_LOG_FUNCTION(CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		REM_LOGGER_INTERNAL(TEXT(""), Suffix, CategoryName, Verbosity, Format, ##__VA_ARGS__) \
	} while (false)

// Log message with optional role name prefix, __FUNCTION__ suffix
#define REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		REM_INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		REM_LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, Format, ##__VA_ARGS__) \
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
		if (auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			ensure(CVar)) \
		{ \
			REM_LOG_ROLE_COND(CVar->GetBool(), Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#define REM_LOG_FUNCTION_CVAR(ConsoleVariableName, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			ensure(CVar)) \
		{ \
			REM_LOG_FUNCTION_COND(CVar->GetBool(), CategoryName, Verbosity, Format, ##__VA_ARGS__) \
		} \
	} while (false)

#define REM_LOG_ROLE_FUNCTION_CVAR(ConsoleVariableName, Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		if (auto* CVar = Rem::FindConsoleVariable(ConsoleVariableName); \
			ensure(CVar)) \
		{ \
			REM_LOG_ROLE_FUNCTION_COND(CVar->GetBool(), Object, CategoryName, Verbosity, Format, ##__VA_ARGS__); \
		} \
	} while (false)

#endif
