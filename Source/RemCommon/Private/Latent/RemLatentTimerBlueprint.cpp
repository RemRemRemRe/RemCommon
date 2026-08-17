// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "Latent/RemLatentTimerBlueprint.h"

#include "Latent/RemLatentTimer.h"

#include <limits>

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemLatentTimerBlueprint)

Rem::Latent::FTimerParameterHelper_Time FRemTimerParameterBlueprintHelper_Time::operator*() const
{
    return {
        .TimeToDelay            = TimeToDelay,
        .LoopCount              = static_cast<uint32>(LoopCount),
        .InitialDelay           = InitialDelay,
        .bMaxOncePerFrame       = bMaxOncePerFrame,
        .bSkipCountingThisFrame = bSkipCountingThisFrame,
    };
}

Rem::Latent::FTimerParameterHelper_Frame FRemTimerParameterBlueprintHelper_Frame::operator*() const
{
    // The blueprint helper uses int32::max() as its "unset" sentinel while the latent
    // action checks uint32::max(); map the sentinel so the default initial delay is
    // preserved (a raw cast would produce 0x7FFFFFFF and delay the timer by ~2^31 frames).
    return {
        .FrameToDelay           = static_cast<uint32>(FrameToDelay),
        .LoopCount              = static_cast<uint32>(LoopCount),
        .InitialDelay           = InitialDelay == std::numeric_limits<int32>::max()
                                      ? std::numeric_limits<uint32>::max()
                                      : static_cast<uint32>(InitialDelay),
        .bSkipCountingThisFrame = bSkipCountingThisFrame,
    };
}
