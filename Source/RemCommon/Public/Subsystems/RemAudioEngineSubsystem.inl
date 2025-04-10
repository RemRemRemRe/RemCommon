// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "AudioDevice.h"
#include "AudioDeviceHandle.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.h"

class UAudioEngineSubsystem;

namespace Rem::Subsystem
{
	template<std::derived_from<UAudioEngineSubsystem> T>
	T* GetAudioEngineSubsystem(const UObject& Object)
	{
		const auto AudioDevice = Object::GetAudioDevice(Object);
		RemCheckCondition(AudioDevice, return nullptr);

		return AudioDevice->GetSubsystem<T>();
	}

	template<std::derived_from<UAudioEngineSubsystem> T>
	T& GetAudioEngineSubsystemRef(const UObject& Object)
	{
		return *GetAudioEngineSubsystem<T>(Object);
	}
}
