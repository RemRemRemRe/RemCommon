// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemNotNull.h"
#include "Engine/Engine.h"
#include "Macro/RemAssertionMacros.h"

class UEngineSubsystem;

namespace Rem::Subsystem
{
	template<std::derived_from<UEngineSubsystem> T>
	T* GetEngineSubsystem()
	{
	    RemEnsureVariable(GEngine, return {});
		return GEngine->GetEngineSubsystem<T>();
	}

	template<std::derived_from<UEngineSubsystem> T>
	auto GetEngineSubsystemNotNull()
	{
		return MakeNotNull(GetEngineSubsystem<T>());
	}
}
