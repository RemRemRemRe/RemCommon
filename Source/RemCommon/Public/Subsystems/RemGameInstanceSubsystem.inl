// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "UObject/Object.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.inl"

class UGameInstance;
class UGameInstanceSubsystem;

namespace Rem::Subsystem
{
	template<typename T>
	requires std::is_base_of_v<UGameInstanceSubsystem, T>
	T* GetGameInstanceSubsystem(const UObject& Object)
	{
		const auto* GameInstance = Object::GetGameInstance(Object);
		RemCheckCondition(GameInstance, return nullptr);

		return GameInstance->GetSubsystem<T>();
	}

	template<typename T>
	requires std::is_base_of_v<UGameInstanceSubsystem, T>
	T& GetGameInstanceSubsystemRef()
	{
		return *GetGameInstanceSubsystem<T>();
	}
}
