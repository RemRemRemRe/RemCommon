// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Engine/Engine.h"

class UEngineSubsystem;

namespace Rem::Subsystem
{
	template<std::derived_from<UEngineSubsystem> T>
	T* GetEngineSubsystem()
	{
		if (GEngine)
		{
			return GEngine->GetEngineSubsystem<T>();
		}

		return nullptr;
	}

	template<std::derived_from<UEngineSubsystem> T>
	T& GetEngineSubsystemRef()
	{
		return *GetEngineSubsystem<T>();
	}
}
