// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Macro/RemFunctionNameUtility.h"

#if REM_WITH_DEVELOPMENT_ONLY_CODE

#define REM_SCENE_QUERY_NAME \
    [] () \
    { \
        static const auto Name = FName{REM_FUNCTION_LINE}; \
        return Name; \
    }()

#else

#define REM_SCENE_QUERY_NAME FName{}

#endif
