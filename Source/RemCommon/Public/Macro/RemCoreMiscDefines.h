// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RemMacroUtilities.h"

#if !UE_BUILD_TEST || !UE_BUILD_SHIPPING

// An alternative for PURE_VIRTUAL which will cause unwanted process terminated
#define REM_VIRTUAL_WARN(CategoryName, ...) \
	{ \
		REM_INITIALIZE_SOURCE_LOCATION_STRING(SourceLocation) \
		UE_LOG(CategoryName, Error, TEXT("Called a virtual function which is not implemented (%s)"), *SourceLocation); \
		__VA_ARGS__ \
	}

#else

#define REM_VIRTUAL_WARN(CategoryName, ...) PURE_VIRTUAL(__FUNCTION__, ##__VA_ARGS__)

#endif
