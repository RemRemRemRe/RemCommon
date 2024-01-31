// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <source_location>

#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING

#define REM_INITIALIZE_NET_DEBUG_STRING(Object, VariableName) \
	const FString VariableName = Rem::GetNetDebugString(Object);

#define REM_INITIALIZE_SOURCE_LOCATION_STRING(VariableName) \
	constexpr std::source_location Rem_Source_Location_Current = std::source_location::current(); \
	const FString VariableName = FString::Format(TEXT("[Function: {0}] [Line: {1}]"), \
	{Rem_Source_Location_Current.function_name(), Rem_Source_Location_Current.line()});

#ifdef REM_WITH_DEVELOPMENT_ONLY_CODE

#define REM_RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#else

#define REM_RETURN_IF_NOT_GAME_WORLD

#endif