// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "RemLifecyclePhase.generated.h"

/** what lifecycle step of one logically continuous event this is */
UENUM(BlueprintType)
enum class ERemLifecyclePhase : uint8
{
    /** the event starts; presentation creates its handlers here */
    Start,

    /** a running event is suspended; handlers keep their state */
    Pause,

    /** a paused event runs again */
    Resume,

    /** the event ends; the handlers created at Start are released */
    Stop,
};
