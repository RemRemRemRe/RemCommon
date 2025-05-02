// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainerOwnerInterface.h"
#include "RemComponentContainer.inl"

#include "RemConcepts.h"

struct FRemComponentBase;

namespace Rem::Component
{

template <std::derived_from<FRemComponentBase> T = FRemComponentBase, std::derived_from<IRemComponentContainerOwnerInterface> TInterface = IRemComponentContainerOwnerInterface>
auto FindComponent(TInterface& Interface) -> decltype(auto)
{
	return Interface.GetComponentContainer().template FindComponent<T>();
}

template <std::derived_from<FRemComponentBase> T = FRemComponentBase, Concepts::is_uobject TObject = UObject>
auto FindComponent(TObject& Object) -> decltype(auto)
{
	auto* Interface = ::Cast<IRemComponentContainerOwnerInterface, TObject>(Object);

	using TResult = decltype(FindComponent<T>(*Interface));
	if (Interface)
	{
		return FindComponent<T>(*Interface);
	}

	return TResult{};
}

}
