// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING

#if REM_WITH_DEVELOPMENT_ONLY_CODE

#define REM_RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#else

#define REM_RETURN_IF_NOT_GAME_WORLD

#endif

#define REM_IF_THEN(Condition, Statement) \
	do \
	{ \
		if (Condition) \
		{ \
			Statement \
		}\
	}\
	while (false)

#define REM_SET_TRUE_IF(Variable) REM_IF_THEN(Variable, Variable = true;)
#define REM_SET_FALSE_IF(Variable) REM_IF_THEN(Variable, Variable = false;)
