// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainer.h"
#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"

template<std::derived_from<FRemComponentBase> T>
T* FRemComponentContainer::FindComponent()
{
	return Rem::Struct::FindStructView<T>(MakeArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemComponentBase> T>
const T* FRemComponentContainer::FindComponent() const
{
	return Rem::Struct::FindConstStructView<T>(MakeConstArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemComponentBase> T>
T* FRemComponentContainer::FindComponent(const int32 Index)
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<T>();
}

template<std::derived_from<FRemComponentBase> T>
const T* FRemComponentContainer::FindComponent(const int32 Index) const
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<const T>();
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
T* FRemComponentContainer::FindComponent(EnumClass Enum)
{
	return FindComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
const T* FRemComponentContainer::FindComponent(EnumClass Enum) const
{
	return FindComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(TFunctionRef<void(T&)> FunctionRef)
{
	Rem::Struct::ForEachStructView(MakeArrayView(Components), FunctionRef);
}

template<std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const
{
	Rem::Struct::ForEachStructView(MakeConstArrayView(Components), FunctionRef);
}
