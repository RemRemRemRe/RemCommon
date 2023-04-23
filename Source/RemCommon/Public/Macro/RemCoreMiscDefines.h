﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RemMacroUtilities.h"

// An alternative for PURE_VIRTUAL which will cause unwanted process terminated
#define REM_VIRTUAL_WARN(CategoryName, ...) \
	{ \
		REM_INITIALIZE_SOURCE_LOCATION_STRING(SourceLocation) \
		UE_LOG(CategoryName, Warning, TEXT("Called a virtual function which is not implemented (%s)"), *SourceLocation); \
		__VA_ARGS__ \
	}
