// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemMacroUtilities.h"

// for Rem::StringFormat
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
		constexpr auto Delimiter = TEXT(' '); \
		TStringBuilder<256> FinalString{}; \
		 \
		FinalString<<TEXT("Frame:"); \
		FinalString<<::GFrameNumber; \
		FinalString<<Delimiter; \
		 \
		if constexpr ( constexpr auto StringView = TEXTVIEW(#Prefix); \
			!StringView.IsEmpty()) \
		{ \
			FinalString<<FString{Prefix}; \
			FinalString<<Delimiter; \
		} \
		 \
		FinalString<<Rem::StringFormat(FormatString, ##__VA_ARGS__); \
		 \
		if constexpr ( constexpr auto StringView = TEXTVIEW(#Suffix); \
			!StringView.IsEmpty()) \
		{ \
			FinalString<<Delimiter; \
			FinalString<<FString{Suffix}; \
		} \
		 \
		UE_LOG(CategoryName, Verbosity, TEXT("%s"), *FinalString); \
	}

// Log message with optional role name prefix
#define REM_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		REM_LOGGER_INTERNAL(std::move(Prefix), /* no suffix */, CategoryName, Verbosity, Format, ##__VA_ARGS__) \
	} while (false)

// Log message with __FUNCTION__ suffix
#define REM_LOG_FUNCTION(CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		REM_LOGGER_INTERNAL(/* no prefix */, std::move(Suffix), CategoryName, Verbosity, Format, ##__VA_ARGS__) \
	} while (false)

// Log message with optional role name prefix, __FUNCTION__ suffix
#define REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		REM_INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		REM_INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		REM_LOGGER_INTERNAL(std::move(Prefix), std::move(Suffix), CategoryName, Verbosity, Format, ##__VA_ARGS__) \
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

#endif
