// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "UObject/Object.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"

class UWorldSubsystem;

namespace Rem::Subsystem
{
	template<std::derived_from<UWorldSubsystem> T>
	T* GetWorldSubsystem(const UObject& Object)
	{
		const auto* World = Object.GetWorld();
		RemCheckCondition(World, return nullptr);

		return World->GetSubsystem<T>();
	}

	template<std::derived_from<UWorldSubsystem> T>
	T& GetWorldSubsystemRef(const UObject& Object)
	{
		return *GetWorldSubsystem<T>(Object);
	}
}
