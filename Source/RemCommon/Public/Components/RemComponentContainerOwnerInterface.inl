// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainerOwnerInterface.h"
#include "RemComponentContainer.inl"

#include "RemConcepts.h"

struct FRemComponentBase;

namespace Rem::Component
{
    
template <Concepts::is_uobject TObject = UObject>
auto GetComponentContainer(TObject& Object) -> decltype(auto)
{
    auto* Interface = ::Cast<IRemComponentContainerOwnerInterface, TObject>(&Object);

    using TResult = std::add_pointer_t<std::remove_reference_t<decltype(Interface->GetComponentContainer())>>;
    if (Interface)
    {
        return &Interface->GetComponentContainer();
    }

    return TResult{};
}
    
template <std::derived_from<FRemComponentBase> T = FRemComponentBase, Concepts::is_uobject TObject = UObject>
auto FindComponent(TObject& Object) -> decltype(auto)
{
    using TResult = decltype(GetComponentContainer<TObject>(Object)->template FindComponent<T>());

    auto* ComponentContainer = GetComponentContainer<TObject>(Object);
    RemCheckVariable(ComponentContainer, return TResult{});
    
    auto ComponentView = ComponentContainer->template FindComponent<T>();

    return ComponentView;
}
    
}
