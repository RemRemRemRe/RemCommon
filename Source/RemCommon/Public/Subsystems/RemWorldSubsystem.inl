// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <concepts>

#include "UObject/Object.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"
#include "RemNotNull.h"

class UWorldSubsystem;

namespace Rem::Subsystem
{
/**
 * the world subsystem of the requested type, resolved from Object's world
 *
 * @tparam T expected type of the subsystem
 * @return the registered subsystem, or nullptr if Object has no world or no subsystem of that type is registered
 * @note this is the nullable half of the accessor pair: a missing subsystem is not reported, use it when
 * the subsystem may be missing. Object's world is still a checked precondition, because without it no
 * lookup is possible at all - RemEnsureVariable reports it and the function returns nullptr
 */
template <std::derived_from<UWorldSubsystem> T>
[[nodiscard]] T* TryGetWorldSubsystem(const TNotNull<const UObject*> Object)
{
    const auto* World = Object->GetWorld();
    RemEnsureVariable(World, return nullptr);

    return World->GetSubsystem<T>();
}

/**
 * the world subsystem of the requested type, checked to exist
 *
 * @tparam T expected type of the subsystem
 * @return the registered subsystem
 * @pre Object has a world and a subsystem of the requested type is registered; a violation is reported
 * through RemEnsureVariable(..., REM_NO_HANDLING) and then through TNotNull's null check
 * (ReportNotNullPtr -> UE_LOGF(LogCore, Fatal, ...)), which terminates the process in Debug/Development
 * @note there is no fallback: a violated precondition leaves the caller without any subsystem, use
 * TryGetWorldSubsystem when the subsystem may be missing
 * @note where the not-null wrapper is compiled out (UE_ENABLE_NOTNULL_WRAPPER == 0, i.e. in Test and
 * Shipping) TNotNull<T*> degenerates to a plain T* and the null check no longer reports, so a violated
 * precondition yields a null pointer - dereferencing it is undefined behavior; the RemEnsureVariable
 * report is likewise compiled out where DO_ENSURE == 0
 */
template <std::derived_from<UWorldSubsystem> T>
[[nodiscard]] TNotNull<T*> GetWorldSubsystem(const TNotNull<const UObject*> Object)
{
    auto* Subsystem = TryGetWorldSubsystem<T>(Object);
    RemEnsureVariable(Subsystem, REM_NO_HANDLING);
    return Subsystem;
}
}
