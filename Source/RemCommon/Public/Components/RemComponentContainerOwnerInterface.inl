// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainerOwnerInterface.h"
#include "RemComponentContainer.inl"

#include "RemConcepts.h"

struct FRemComponentBase;

namespace Rem::Component
{

template <std::derived_from<FRemComponentBase> T = FRemComponentBase, std::derived_from<IRemComponentContainerOwnerInterface> TInterface = IRemComponentContainerOwnerInterface>
auto GetComponent(TInterface& Interface) -> decltype(auto)
{
	return Interface.GetComponentContainer().template GetComponent<T>();
}

template <std::derived_from<FRemComponentBase> T = FRemComponentBase, Concepts::is_uobject TObject = UObject>
auto GetComponent(TObject& Object) -> decltype(auto)
{
	auto* Interface = ::Cast<IRemComponentContainerOwnerInterface, TObject>(Object);

	using TResult = decltype(GetComponent<T>(*Interface));
	if (Interface)
	{
		return GetComponent<T>(*Interface);
	}

	return TResult{};
}

}
