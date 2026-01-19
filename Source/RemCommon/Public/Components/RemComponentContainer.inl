// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainer.h"
#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"

template<std::derived_from<FRemComponentBase> T>
auto FRemComponentContainer::FindComponent()
{
    return Rem::Struct::FindStructView<T>(MakeArrayView(Components));
}

template<std::derived_from<FRemComponentBase> T>
auto FRemComponentContainer::FindComponent() const
{
	return Rem::Struct::FindConstStructView<T>(MakeConstArrayView(Components));
}

template<std::derived_from<FRemComponentBase> T>
auto FRemComponentContainer::FindComponent(const int32 Index)
{
    RemCheckCondition(Components.IsValidIndex(Index), return {});

    return Rem::Struct::TryMakeView<T>(FStructView{Components[Index].GetScriptStruct(), Components[Index].GetMutableMemory()});
}

template<std::derived_from<FRemComponentBase> T>
auto FRemComponentContainer::FindComponent(const int32 Index) const
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Rem::Struct::TryMakeView<T>(FConstStructView{Components[Index].GetScriptStruct(), Components[Index].GetMemory()});
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
auto FRemComponentContainer::FindComponent(EnumClass Enum)
{
	return FindComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentBase> T, Rem::Concepts::is_scoped_enum EnumClass>
auto FRemComponentContainer::FindComponent(EnumClass Enum) const
{
	return FindComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(TFunctionRef<void(T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef)
{
	Rem::Struct::ForEachStructView(MakeArrayView(Components), FunctionRef);
}

template <std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(TFunctionRef<void(const T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef) const
{
    Rem::Struct::ForEachStructView(MakeConstArrayView(Components), FunctionRef);
}

template<std::derived_from<FRemComponentBase> TComponentType>
int32 FRemComponentContainer::GetComponentIndex() const
{
    const auto* Struct = TComponentType::StaticStruct();
    
    for (int32 Index = 0; Index < Components.Num(); ++Index)
    {
        if (Struct == Components[Index].GetScriptStruct())
        {
            return Index;
        }
    }
    
    return INDEX_NONE;
}

template<Rem::Concepts::is_uobject TOwner>
TOwner* FRemComponentContainer::GetOwner() const
{
    return Cast<TOwner>(Owner);
}
