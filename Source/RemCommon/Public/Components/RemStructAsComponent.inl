// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructAsComponent.h"
#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"

template<std::derived_from<FRemStructAsComponentBase> T>
T* FRemStructAsComponents::GetComponent()
{
	return Rem::Struct::GetStructView<T>(MakeArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemStructAsComponentBase> T>
const T* FRemStructAsComponents::GetComponent() const
{
	return Rem::Struct::GetConstStructView<T>(MakeConstArrayView(Components)).GetPtr();
}

template<std::derived_from<FRemStructAsComponentBase> T>
T* FRemStructAsComponents::GetComponent(const int32 Index)
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<T>();
}

template<std::derived_from<FRemStructAsComponentBase> T>
const T* FRemStructAsComponents::GetComponent(const int32 Index) const
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Components[Index].GetPtr<const T>();
}

template<std::derived_from<FRemStructAsComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
T* FRemStructAsComponents::GetComponent(EnumClass Enum)
{
	return GetComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemStructAsComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
const T* FRemStructAsComponents::GetComponent(EnumClass Enum) const
{
	return GetComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemStructAsComponentBase> T>
void FRemStructAsComponents::ForEachComponent(TFunctionRef<void(T&)> FunctionRef)
{
	Rem::Struct::ForEachStructView(MakeArrayView(Components), FunctionRef);
}

template<std::derived_from<FRemStructAsComponentBase> T>
void FRemStructAsComponents::ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const
{
	Rem::Struct::ForEachStructView(MakeConstArrayView(Components), FunctionRef);
}
