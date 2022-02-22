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
		FString OriginalStr = FString::Printf(Format, __VA_ARGS__); \
		FString FinalStr = Prefix + OriginalStr + Suffix; \
		UE_LOG(CategoryName, Verbosity, TEXT("%s"), *FinalStr); \
	}

#define DECLARE_ROLE_PREFIX(ActorOrComponent) \
	FString Prefix; \
	if (ActorOrComponent != nullptr) \
	{ \
		if (IS_NET_MODE(ActorOrComponent, NM_DedicatedServer) || IS_NET_MODE(ActorOrComponent, NM_ListenServer)) \
		{ \
			Prefix = FString::Printf(TEXT("Server  : ")); \
		} \
		else if (IS_NET_MODE(ActorOrComponent, NM_Client)) \
		{ \
			Prefix = FString::Printf(TEXT("Client %d: "), GPlayInEditorID); \
		} \
	}

// Log message with optional role name prefix
#define UE_LOG_ROLE(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	{ \
		DECLARE_ROLE_PREFIX(ActorOrComponent); \
		\
		LOGGER_INTERNAL(Prefix, TEXT(""), CategoryName, Verbosity, Format, __VA_ARGS__) \
	}

// Log message with __FUNCTION__ suffix
#define UE_LOG_FUNCTION(CategoryName, Verbosity, Format, ...) \
	{ \
		FString Prefix; \
		LOGGER_INTERNAL(Prefix, FString(__FUNCTION__), CategoryName, Verbosity, Format, __VA_ARGS__) \
	}

// Log message with optional role name prefix, __FUNCTION__ suffix
#define UE_LOG_ROLE_FUNCTION(ActorOrComponent, CategoryName, Verbosity, Format, ...) \
	{ \
		DECLARE_ROLE_PREFIX(ActorOrComponent); \
		\
		LOGGER_INTERNAL(Prefix, FString(__FUNCTION__), CategoryName, Verbosity, Format, __VA_ARGS__) \
	}

#define BOOL_TO_STRING(Condition) ( (Condition) ? TEXT("True") : TEXT("False") )

#endif
