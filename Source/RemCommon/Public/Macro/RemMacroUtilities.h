// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <source_location>

#define REM_WITH_DEVELOPMENT_ONLY_CODE !UE_BUILD_SHIPPING

#define REM_INITIALIZE_NET_DEBUG_STRING(Object, VariableName) \
	const FString VariableName = Rem::Common::GetNetDebugString(Object);

#define REM_INITIALIZE_SOURCE_LOCATION_STRING(VariableName) \
	constexpr std::source_location Rem_Source_Location_Current = std::source_location::current(); \
	const FString VariableName = FString::Format(TEXT("[File: {0}] [Function: {1}] [Line: {2}]"), \
	{Rem_Source_Location_Current.file_name(), Rem_Source_Location_Current.function_name(), Rem_Source_Location_Current.line()});
