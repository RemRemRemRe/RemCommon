// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "AudioDevice.h"
#include "AudioDeviceHandle.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.h"

class UAudioEngineSubsystem;

namespace Rem::Subsystem
{
	template<typename T>
	requires std::is_base_of_v<UAudioEngineSubsystem, T>
	T* GetAudioEngineSubsystem(const UObject& Object)
	{
		const auto AudioDevice = Object::GetAudioDevice(Object);
		RemCheckCondition(AudioDevice, return nullptr);

		return AudioDevice->GetSubsystem<T>();
	}

	template<typename T>
	requires std::is_base_of_v<UAudioEngineSubsystem, T>
	T& GetAudioEngineSubsystemRef(const UObject& Object)
	{
		return *GetAudioEngineSubsystem<T>(Object);
	}
}
