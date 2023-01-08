// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MacroUtilities.h"

// Extended log macro

#if NO_LOGGING

#define UE_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, __VA_ARGS__)

#define UE_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, __VA_ARGS__)

#define UE_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, __VA_ARGS__)

#else

#define LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, FormatString, ...) \
	{ \
		const FString OriginalStr = Common::StringFormat(FormatString, __VA_ARGS__); \
		const FString FinalStr = FString::Format(TEXT("{0} {1} {2}"), {Prefix, OriginalStr, Suffix}); \
		UE_LOG(CategoryName, Verbosity, TEXT("%s"), *FinalStr); \
	}

// Log message with optional role name prefix
#define UE_LOG_ROLE(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		LOGGER_INTERNAL(Prefix, TEXT(""), CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

// Log message with __FUNCTION__ suffix
#define UE_LOG_FUNCTION(CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		LOGGER_INTERNAL(TEXT(""), Suffix, CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

// Log message with optional role name prefix, __FUNCTION__ suffix
#define UE_LOG_ROLE_FUNCTION(Object, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		INITIALIZE_NET_DEBUG_STRING(Object, Prefix) \
		\
		INITIALIZE_SOURCE_LOCATION_STRING(Suffix) \
		\
		LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

#endif
