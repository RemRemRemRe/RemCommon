// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc.h"

// Extended log macro

#if NO_LOGGING

#define UE_LOG_ROLE(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, __VA_ARGS__)

#define UE_LOG_ROLE_FUNCTION(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	UE_LOG(CategoryName, Verbosity, Format, __VA_ARGS__)

#else

#define LOGGER_INTERNAL(Prefix, Suffix, CategoryName, Verbosity, Format, ...) \
	{ \
		const FString OriginalStr = FString::Printf(Format, __VA_ARGS__); \
		const FString FinalStr = Prefix + OriginalStr + TEXT(" ") + Suffix; \
		UE_LOG(CategoryName, Verbosity, TEXT("%s"), *FinalStr); \
	}

#define DECLARE_NET_MODE_AND_ROLE_PREFIX(ActorOrComponent) \
	FString Prefix; \
	if (Common::IsValid(ActorOrComponent)) \
	{ \
		if (Common::IsNetMode(ActorOrComponent, NM_DedicatedServer) || Common::IsNetMode(ActorOrComponent, NM_ListenServer)) \
		{ \
			Prefix = FString::Printf(TEXT("Server   ")); \
		} \
		else \
		{ \
			Prefix = FString::Printf(TEXT("Client %d "), GPlayInEditorID); \
		} \
		\
		if (const FString RoleString = Common::GetNetRoleString(ActorOrComponent); \
			!RoleString.IsEmpty()) \
		{ \
			Prefix += FString::Printf(TEXT("%s : "), *RoleString); \
		} \
		else \
		{ \
			Prefix += TEXT(": "); \
		}\
	}

// Log message with optional role name prefix
#define UE_LOG_ROLE(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		DECLARE_NET_MODE_AND_ROLE_PREFIX(ActorOrComponent) \
		\
		LOGGER_INTERNAL(Prefix, TEXT(""), CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

// Log message with __FUNCTION__ suffix
#define UE_LOG_FUNCTION(CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		FString Prefix; \
		LOGGER_INTERNAL(Prefix, FString(__FUNCTION__), CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

// Log message with optional role name prefix, __FUNCTION__ suffix
#define UE_LOG_ROLE_FUNCTION(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	do \
	{ \
		DECLARE_NET_MODE_AND_ROLE_PREFIX(ActorOrComponent) \
		\
		LOGGER_INTERNAL(Prefix, FString(__FUNCTION__), CategoryName, Verbosity, Format, __VA_ARGS__) \
	} while (false)

#define BOOL_TO_STRING(Condition) ( (Condition) ? TEXT("True") : TEXT("False") )

#endif
