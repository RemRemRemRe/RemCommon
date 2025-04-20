// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainer.h"
#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"

template<std::derived_from<FRemComponentBase> T>
T* FRemComponentContainer::GetComponent()
{
	return Rem::Struct::GetStructView<T>(MakeArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemComponentBase> T>
const T* FRemComponentContainer::GetComponent() const
{
	return Rem::Struct::GetConstStructView<T>(MakeConstArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemComponentBase> T>
T* FRemComponentContainer::GetComponent(const int32 Index)
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<T>();
}

template<std::derived_from<FRemComponentBase> T>
const T* FRemComponentContainer::GetComponent(const int32 Index) const
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<const T>();
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
T* FRemComponentContainer::GetComponent(EnumClass Enum)
{
	return GetComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
const T* FRemComponentContainer::GetComponent(EnumClass Enum) const
{
	return GetComponent<T>(static_cast<int32>(Enum));
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
