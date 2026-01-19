// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentViewContainer.h"
#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"

template <std::derived_from<FRemComponentViewBase> TComponentType>
TComponentType::FInstanceDataType* FRemComponentViewBase::GetInstanceData(const FContext& Context) const
{
    const auto DataView = Context.OwnerInstance.GetInstanceDataView(Context.ComponentIndex);
    
    return DataView.GetPtr<typename TComponentType::FInstanceDataType>();
}

template<std::derived_from<FRemComponentViewBase> T>
auto FRemComponentViewContainerInstance::FindComponent() const
{
	return Rem::Struct::FindConstStructView<T>(MakeConstArrayView(Components));
}

template<std::derived_from<FRemComponentViewBase> T>
auto FRemComponentViewContainerInstance::FindComponent(const int32 Index) const
{
	RemCheckCondition(Components.IsValidIndex(Index), return {});

	return Rem::Struct::TryMakeView<T>(FConstStructView{Components[Index].GetScriptStruct(), Components[Index].GetMemory()});
}

template<std::derived_from<FRemComponentViewBase> T, Rem::Concepts::is_scoped_enum EnumClass>
auto FRemComponentViewContainerInstance::FindComponent(EnumClass Enum) const
{
	return FindComponent<T>(static_cast<int32>(Enum));
}

template<std::derived_from<FRemComponentViewBase> T>
void FRemComponentViewContainerInstance::ForEachComponent(
    TFunctionRef<void(const T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef) const
{
	Rem::Struct::ForEachStructView(MakeConstArrayView(Components), FunctionRef);
}

template <std::derived_from<FRemComponentViewBase> TComponentType>
TStructView<typename TComponentType::FInstanceDataType> FRemComponentViewContainerInstance::GetInstanceData(const int32 ComponentIndex)
{
    const auto InstanceData = GetInstanceDataView(ComponentIndex);
    return Rem::Struct::TryMakeView<typename TComponentType::FInstanceDataType>(InstanceData);
}

template <std::derived_from<FRemComponentViewBase> TComponentType>
TStructView<typename TComponentType::FInstanceDataType> FRemComponentViewContainerInstance::GetInstanceData()
{
    const auto ComponentIndex = GetComponentIndex<TComponentType>();
    RemCheckCondition(ComponentIndex != INDEX_NONE, return {});
    
    auto InstanceData = GetInstanceDataView(ComponentIndex);
    return Rem::Struct::TryMakeView<typename TComponentType::FInstanceDataType>(InstanceData);
}

template <std::derived_from<FRemComponentViewBase> TComponentType>
int32 FRemComponentViewContainerInstance::GetComponentIndex() const
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

template <Rem::Concepts::is_uobject TOwner>
TOwner* FRemComponentViewContainerInstance::GetOwner() const
{
    return Cast<TOwner>(Owner);
}
