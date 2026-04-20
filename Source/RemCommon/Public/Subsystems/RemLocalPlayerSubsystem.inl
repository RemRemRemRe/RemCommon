// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "UObject/Object.h"
#include "Engine/LocalPlayer.h"
#include "Macro/RemAssertionMacros.h"
#include "Object/RemObjectStatics.inl"

class ULocalPlayerSubsystem;

namespace Rem::Subsystem
{
template <std::derived_from<ULocalPlayerSubsystem> T>
T* GetLocalPlayerSubsystem(const TNotNull<const UObject*> Object)
{
    const auto* LocalPlayer = Object::GetFirstLocalPlayer(Object);
    RemCheckCondition(LocalPlayer, return nullptr);

    return LocalPlayer->GetSubsystem<T>();
}

template <std::derived_from<ULocalPlayerSubsystem> T>
auto GetLocalPlayerSubsystemNotNull(const TNotNull<const UObject*> Object)
{
    return MakeNotNull(GetLocalPlayerSubsystem<T>(Object));
}
}
