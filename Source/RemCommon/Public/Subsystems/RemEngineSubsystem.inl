// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "Engine/Engine.h"

class UEngineSubsystem;

namespace Rem::Subsystem
{
	template<typename T>
	requires std::is_base_of_v<UEngineSubsystem, T>
	T* GetEngineSubsystem()
	{
		if (GEngine)
		{
			return GEngine->GetEngineSubsystem<T>();
		}

		return nullptr;
	}

	template<typename T>
	requires std::is_base_of_v<UEngineSubsystem, T>
	T& GetEngineSubsystemRef()
	{
		return *GetEngineSubsystem<T>();
	}
}
