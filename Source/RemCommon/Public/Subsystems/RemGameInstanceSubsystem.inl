// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <concepts>
#include <type_traits>

#include "UObject/Object.h"
#include "Engine/GameInstance.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.inl"

class UGameInstance;
class UGameInstanceSubsystem;

namespace Rem::Subsystem
{
/**
 * the game-instance subsystem of the requested type, resolved from Object's game instance
 *
 * @tparam T expected type of the subsystem
 * @return the registered subsystem, or nullptr if no subsystem of that type is registered
 * @note this is the nullable half of the accessor pair: a missing subsystem is not reported, use it when
 * the subsystem may be missing. The game instance itself is still a checked precondition, because
 * without it no lookup is possible at all - RemEnsureVariable reports it and the function returns nullptr
 */
template <std::derived_from<UGameInstanceSubsystem> T>
[[nodiscard]] T* TryGetGameInstanceSubsystem(const TNotNull<const UObject*> Object)
{
    const auto* GameInstance = Object::GetGameInstance(Object);
    RemEnsureVariable(GameInstance, return nullptr);

    return GameInstance->GetSubsystem<T>();
}

/**
 * the game-instance subsystem of the requested type, checked to exist
 *
 * @tparam T expected type of the subsystem
 * @return the registered subsystem
 * @pre a game instance resolves from Object and a subsystem of the requested type is registered; a
 * violation is reported through RemEnsureVariable(..., REM_NO_HANDLING) and then through TNotNull's
 * null check (ReportNotNullPtr -> UE_LOGF(LogCore, Fatal, ...)), which terminates the process in
 * Debug/Development
 * @note there is no fallback: a violated precondition leaves the caller without any subsystem, use
 * TryGetGameInstanceSubsystem when the subsystem may be missing
 * @note where the not-null wrapper is compiled out (UE_ENABLE_NOTNULL_WRAPPER == 0, i.e. in Test and
 * Shipping) TNotNull<T*> degenerates to a plain T* and the null check no longer reports, so a violated
 * precondition yields a null pointer - dereferencing it is undefined behavior; the RemEnsureVariable
 * report is likewise compiled out where DO_ENSURE == 0
 */
template <std::derived_from<UGameInstanceSubsystem> T>
[[nodiscard]] TNotNull<T*> GetGameInstanceSubsystem(const TNotNull<const UObject*> Object)
{
    auto* Subsystem = TryGetGameInstanceSubsystem<T>(Object);
    RemEnsureVariable(Subsystem, REM_NO_HANDLING);
    return Subsystem;
}
}
