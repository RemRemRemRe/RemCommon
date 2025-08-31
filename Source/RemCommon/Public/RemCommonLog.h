// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"
#include "Macro/RemMacroUtilities.h"

#define REM_API REMCOMMON_API

REM_API DECLARE_LOG_CATEGORY_EXTERN(LogRemCommon, REM_DEFAULT_LOG_VERBOSITY, REM_MAX_LOG_VERBOSITY)

#undef REM_API
