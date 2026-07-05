// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainerOwnerInterface.h"
#include "RemComponentContainer.inl"
#include "RemNotNull.h"
#include "Misc/Optional.h"

#include "RemConcepts.h"

struct FRemComponentBase;

namespace Rem::Component
{

template <CUObject TObject = UObject>
decltype(auto) GetComponentContainer(TNotNull<TObject*> Object)
{
    using FResult = TOptional<decltype(::Cast<IRemComponentContainerOwnerInterface>(Object)->GetComponentContainer())>;

    auto* Interface = ::Cast<IRemComponentContainerOwnerInterface>(Object);
    RemEnsureVariable(REM_NO_ASSERTION, Interface, return FResult{});

    return FResult{Interface->GetComponentContainer()};
}

template <std::derived_from<FRemComponentBase> T = FRemComponentBase, CUObject TObject = UObject>
decltype(auto) FindComponent(TNotNull<TObject*> Object)
{
    using TResult = decltype(GetComponentContainer(Object).GetValue()->template FindComponent<T>());

    auto ComponentContainer = GetComponentContainer(Object);
    RemEnsureCondition(REM_NO_ASSERTION, ComponentContainer, return TResult{});

    auto ComponentView = ComponentContainer.GetValue()->template FindComponent<T>();

    static_assert(std::is_same_v<decltype(ComponentView), TResult>, "return type missing match");

    return ComponentView;
}

}
