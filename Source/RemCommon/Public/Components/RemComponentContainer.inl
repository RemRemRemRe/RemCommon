// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemComponentContainer.h"

#include "Macro/RemAssertionMacros.h"
#include "Struct/RemInstancedStructStaics.inl"
#include "Components/ActorComponent.h"

template <Rem::CUObject TOwner>
[[nodiscard]] TOwner* FRemComponentBase::GetOwner(const FContext& Context)
{
    return Context.OwnerInstance->GetOwner<TOwner>();
}

template <Rem::CAActor TOwnerActor>
[[nodiscard]] TOwnerActor* FRemComponentBase::GetOwnerOfOwner(const FContext& Context)
{
    auto* Owner = GetOwner<UActorComponent>(Context);
    RemCheckVariable(Owner, return {});

    return Owner->GetOwner<TOwnerActor>();
}

template <std::derived_from<FRemComponentBase> T>
[[nodiscard]] auto FRemComponentContainer::FindComponent()
{
    return Rem::Struct::FindStructView<T>(MakeArrayView(Components)).template Get<0>();
}

template <std::derived_from<FRemComponentBase> T>
[[nodiscard]] auto FRemComponentContainer::FindComponent() const
{
    return Rem::Struct::FindConstStructView<T>(MakeConstArrayView(Components)).template Get<0>();
}

template <std::derived_from<FRemComponentBase> T>
[[nodiscard]] auto FRemComponentContainer::FindComponent(const int32 Index)
{
    RemCheckCondition(Components.IsValidIndex(Index), return TStructView<T>{});

    return Rem::Struct::TryMakeView<T>(FStructView{Components[Index].GetScriptStruct(),
        Components[Index].GetMutableMemory()
    });
}

template <std::derived_from<FRemComponentBase> T>
[[nodiscard]] auto FRemComponentContainer::FindComponent(const int32 Index) const
{
    RemCheckCondition(Components.IsValidIndex(Index), return TConstStructView<T>{});

    return Rem::Struct::TryMakeView<T>(FConstStructView{Components[Index].GetScriptStruct(),
        Components[Index].GetMemory()
    });
}

template <std::derived_from<FRemComponentBase> T, Rem::CScopedEnum EnumClass>
[[nodiscard]] auto FRemComponentContainer::FindComponent(EnumClass Enum)
{
    return FindComponent<T>(static_cast<int32>(Enum));
}

template <std::derived_from<FRemComponentBase> T, Rem::CScopedEnum EnumClass>
[[nodiscard]] auto FRemComponentContainer::FindComponent(EnumClass Enum) const
{
    return FindComponent<T>(static_cast<int32>(Enum));
}

template <std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(
    TFunctionRef<void(T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef)
{
    Rem::Struct::ForEachStructView(MakeArrayView(Components), FunctionRef);
}

template <std::derived_from<FRemComponentBase> T>
void FRemComponentContainer::ForEachComponent(
    TFunctionRef<void(const T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef) const
{
    Rem::Struct::ForEachStructView(MakeConstArrayView(Components), FunctionRef);
}

template <std::derived_from<FRemComponentBase> TComponentType>
int32 FRemComponentContainer::GetComponentIndex() const
{
    return Rem::Struct::FindConstStructView<TComponentType>(MakeConstArrayView(Components)).template Get<1>();
}

template <Rem::CUObject TOwner>
TOwner* FRemComponentContainer::GetOwner() const
{
    return Cast<TOwner>(Owner);
}
