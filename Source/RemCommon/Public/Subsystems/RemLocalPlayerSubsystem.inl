// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "UObject/Object.h"
#include "Engine/LocalPlayer.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.inl"

class ULocalPlayerSubsystem;

namespace Rem::Subsystem
{
	template<typename T>
	requires std::is_base_of_v<ULocalPlayerSubsystem, T>
	T* GetLocalPlayerSubsystem(const UObject& Object)
	{
		const auto* LocalPlayer = Object::GetFirstLocalPlayer(Object);
		RemCheckCondition(LocalPlayer, return nullptr);

		return LocalPlayer->GetSubsystem<T>();
	}

	template<typename T>
	requires std::is_base_of_v<ULocalPlayerSubsystem, T>
	T& GetLocalPlayerSubsystemRef(const UObject& Object)
	{
		return *GetLocalPlayerSubsystem<T>(Object);
	}
}
