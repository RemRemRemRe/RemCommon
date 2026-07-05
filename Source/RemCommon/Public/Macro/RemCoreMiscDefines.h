// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemMacroUtilities.h"
#include "Macro/RemFunctionNameUtility.h"

#if REM_WITH_DEVELOPMENT_ONLY_CODE

// An alternative for PURE_VIRTUAL which will cause unwanted process terminated
#define REM_VIRTUAL_WARN(CategoryName, ...) \
	{ \
		UE_LOGF(CategoryName, Error, "Frame:%d Called a virtual function which is not implemented (%hs)", ::GFrameNumber, REM_FUNCTION_LINE_FORMATED); \
		__VA_ARGS__ \
	}

#else

#include "Misc/CoreMiscDefines.h"

#define REM_VIRTUAL_WARN(CategoryName, ...) PURE_VIRTUAL(__FUNCTION__, ##__VA_ARGS__)

#endif
