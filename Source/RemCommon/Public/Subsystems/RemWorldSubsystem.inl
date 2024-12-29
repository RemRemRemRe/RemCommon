// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "UObject/Object.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"

class UWorldSubsystem;

namespace Rem::Subsystem
{
	template<typename T>
	requires std::is_base_of_v<UWorldSubsystem, T>
	T* GetWorldSubsystem(const UObject& Object)
	{
		const auto* World = Object.GetWorld();
		RemCheckCondition(World, return nullptr);

		return World->GetSubsystem<T>();
	}

	template<typename T>
	requires std::is_base_of_v<UWorldSubsystem, T>
	T& GetWorldSubsystemRef(const UObject& Object)
	{
		return *GetWorldSubsystem<T>(Object);
	}
}
