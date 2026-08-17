// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemTestWorld.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "HAL/ThreadManager.h"
#include "Misc/App.h"
#include "RenderingThread.h"

namespace Rem::Test
{
FRemTestWorld::FRemTestWorld()
    : World(UWorld::CreateWorld(EWorldType::Game, false, TEXT("RemTestWorld")))
{
    check(IsInGameThread());

    auto& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
    WorldContext.SetCurrentWorld(World);
    PrevWorld       = &*GWorld;
    OldFrameCounter = GFrameCounter;
    GWorld          = World;

    World->InitializeActorsForPlay(FURL{});
    auto* Settings = World->GetWorldSettings();
    Settings->MinUndilatedFrameTime = 0.0001f;
    Settings->MaxUndilatedFrameTime = 10.0f;
    World->BeginPlay();
}

FRemTestWorld::~FRemTestWorld()
{
    GEngine->DestroyWorldContext(World);
    World->DestroyWorld(true);
    GWorld          = PrevWorld;
    GFrameCounter   = OldFrameCounter;

    // process render-thread pending cleanups before garbage collection frees
    // objects the render thread may still hold in its deferred destroy queue
    FlushRenderingCommands();

    CollectGarbage(RF_NoFlags, true);
}

void FRemTestWorld::Tick(const float DeltaSeconds)
{
    check(IsInGameThread());

    World->Tick(LEVELTICK_All, DeltaSeconds);
    EndTick();

    FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);

    // FTSTicker is used by various engine modules; mirrors FEngineLoop::Tick()
    FTSTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
    FThreadManager::Get().Tick();
    GEngine->TickDeferredCommands();
}

void FRemTestWorld::EndTick()
{
    check(IsInGameThread());
    ++GFrameCounter;
}
}
