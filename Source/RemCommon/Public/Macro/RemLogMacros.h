// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemMacroUtilities.h"
#include "RemMisc.h"

// Extended log macro

#if NO_LOGGING

#define REM_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)

#define REM_LOG_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)

#define REM_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__)

#else

#define REM_LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, FormatString, ...) \
	{ \
		const FString OriginalStr = Rem::StringFormat(FormatString, ##__VA_ARGS__); \
		const FString FinalStr = FString::Format(TEXT("{0} {1} {2}"), {Prefix, OriginalStr, Suffix}); \
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

#endif
