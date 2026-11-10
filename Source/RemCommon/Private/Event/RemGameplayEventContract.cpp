// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Event/RemGameplayEventContract.h"

#include "CoreGlobals.h"
#include "Misc/AssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemGameplayEventContract)

namespace
{
/** the id the next producer call gets; 1 onwards, because 0 means "no event" to a consumer */
uint32 NextGameplayEventId{1};
}

uint32 AllocateGameplayEventId()
{
    // event dispatch happens on the game thread only, so the counter needs no atomicity
    check(IsInGameThread());

    return NextGameplayEventId++;
}
