// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Rem::Test
{
/**
 * Test-only world: creates a Game world, makes it the current GWorld and ticks it
 * manually. Latent actions (Rem::Latent timers), tick groups and FTSTicker all run
 * through Tick(), so tests can drive frame-based systems without PIE.
 *
 * Usage (RAII):
 *     Rem::Test::FRemTestWorld World;
 *     auto* Actor = World->SpawnActor<AActor>();
 *     World.Tick(0.1f); // advance one frame
 *
 * @note not reentrant: only one FRemTestWorld may be alive at a time.
 */
class REMCOMMONTEST_API FRemTestWorld
{
private:
    UWorld* World{};
    UWorld* PrevWorld{};
    uint32 OldFrameCounter{};

public:
    FRemTestWorld();
    ~FRemTestWorld();

    UE_NONCOPYABLE(FRemTestWorld);

    UWorld* operator->() const noexcept
    {
        return World;
    }

    operator UWorld*() const noexcept
    {
        return World;
    }

    /** Advances the world by one frame (latent actions, ticks, FTSTicker). */
    void Tick(float DeltaSeconds = 0.1f);

    /** Ends the current frame (increments GFrameCounter), same as engine loop. */
    void EndTick();
};
}
