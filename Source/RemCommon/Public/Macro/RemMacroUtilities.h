// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING

#define REM_INITIALIZE_NET_DEBUG_STRING(Object, VariableName) \
	const auto VariableName = Rem::GetNetDebugString(Object);

#define REM_INITIALIZE_SOURCE_LOCATION_STRING(VariableName) \
	const auto VariableName = FString::Format(TEXT("[Function: {0}] [Line: {1}]"), \
	{__FUNCTION__, __LINE__});

#ifdef REM_WITH_DEVELOPMENT_ONLY_CODE

#define REM_RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#else

#define REM_RETURN_IF_NOT_GAME_WORLD

#endif
