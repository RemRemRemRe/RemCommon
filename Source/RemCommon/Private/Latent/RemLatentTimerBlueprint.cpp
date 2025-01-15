// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "Latent/RemLatentTimerBlueprint.h"

#include "Latent/RemLatentTimer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemLatentTimerBlueprint)

Rem::Latent::FTimerParameterHelper_Time FTimerParameterBlueprintHelper_Time::operator*() const
{
	return {
		.TimeToDelay = TimeToDelay,
		.LoopCount = static_cast<uint32>(LoopCount),
		.InitialDelay = InitialDelay,
		.bMaxOncePerFrame = bMaxOncePerFrame,
		.bSkipCountingThisFrame = bSkipCountingThisFrame,
	};
}

Rem::Latent::FTimerParameterHelper_Frame FTimerParameterBlueprintHelper_Frame::operator*() const
{
	return {
		.FrameToDelay = static_cast<uint32>(FrameToDelay),
		.LoopCount = static_cast<uint32>(LoopCount),
		.InitialDelay = static_cast<uint32>(InitialDelay),
		.bSkipCountingThisFrame = bSkipCountingThisFrame,
	};
}
