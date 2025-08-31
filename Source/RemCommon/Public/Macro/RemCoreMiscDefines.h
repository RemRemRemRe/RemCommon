// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemLogMacros.h"

#if REM_WITH_DEVELOPMENT_ONLY_CODE

// An alternative for PURE_VIRTUAL which will cause unwanted process terminated
#define REM_VIRTUAL_WARN(CategoryName, ...) \
	{ \
		REM_INITIALIZE_SOURCE_LOCATION_STRING(SourceLocation) \
		UE_LOG(CategoryName, Error, TEXT("Frame:%d Called a virtual function which is not implemented (%s)"), ::GFrameNumber, *SourceLocation); \
		__VA_ARGS__ \
	}

#else

#include "Misc/CoreMiscDefines.h"

#define REM_VIRTUAL_WARN(CategoryName, ...) PURE_VIRTUAL(__FUNCTION__, ##__VA_ARGS__)

#endif
