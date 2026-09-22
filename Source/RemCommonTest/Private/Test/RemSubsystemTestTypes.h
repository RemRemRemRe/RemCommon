// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/AudioEngineSubsystem.h"
#include "Subsystems/EngineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Subsystems/WorldSubsystem.h"

#include "RemSubsystemTestTypes.generated.h"

namespace Rem::Common::Private
{
// one concrete type per subsystem family: the accessor pairs need an instance of an exact type that their
// context's own subsystem collection has already registered

/**
 * engine subsystem the engine's collection creates for this module
 */
UCLASS()
class URemTestEngineSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()
};

/**
 * world subsystem each world's collection creates
 */
UCLASS()
class URemTestWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
};

/**
 * game-instance subsystem a game instance's collection creates
 */
UCLASS()
class URemTestGameInstanceSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
};

/**
 * local-player subsystem a local player's collection creates
 */
UCLASS()
class URemTestLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()
};

/**
 * audio-engine subsystem the audio device's collection creates
 */
UCLASS()
class URemTestAudioEngineSubsystem : public UAudioEngineSubsystem
{
    GENERATED_BODY()
};

// the same five families, one type each that its collection must never create: the miss case of the accessor
// pairs. ShouldCreateSubsystem() is queried on the CDO before instantiation, so a false answer means the type
// is absent while the context is still resolvable

/**
 * engine subsystem that is never created
 */
UCLASS()
class URemTestAbsentEngineSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject*) const override { return false; }
};

/**
 * world subsystem that is never created
 */
UCLASS()
class URemTestAbsentWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject*) const override { return false; }
};

/**
 * game-instance subsystem that is never created
 */
UCLASS()
class URemTestAbsentGameInstanceSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject*) const override { return false; }
};

/**
 * local-player subsystem that is never created
 */
UCLASS()
class URemTestAbsentLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject*) const override { return false; }
};

/**
 * audio-engine subsystem that is never created
 */
UCLASS()
class URemTestAbsentAudioEngineSubsystem : public UAudioEngineSubsystem
{
    GENERATED_BODY()

public:
    virtual bool ShouldCreateSubsystem(UObject*) const override { return false; }
};
}
