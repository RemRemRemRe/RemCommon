// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING

#if REM_WITH_DEVELOPMENT_ONLY_CODE

#define REM_RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#define REM_DEFAULT_LOG_VERBOSITY Log
#define REM_MAX_LOG_VERBOSITY All

#else

#define REM_RETURN_IF_NOT_GAME_WORLD

#define REM_DEFAULT_LOG_VERBOSITY Display
#define REM_MAX_LOG_VERBOSITY All

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

#define REM_GET_TYPE_STRING(Type) \
	((void) sizeof UEAsserts_Private::GetMemberNameCheckedJunk(static_cast<Type*>(nullptr)), TEXTVIEW(#Type))
