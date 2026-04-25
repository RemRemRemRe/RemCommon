// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructConcepts.h"
#include "RemConcepts.h"

#include "StructUtils/StructView.h"

namespace Rem::Struct
{
template <CHasStaticStruct TStructType, typename StructView>
    requires (std::is_same_v<StructView, FStructView> || std::is_same_v<StructView, FConstStructView>)
auto MakeView(const StructView View)
{
    using FTResult = std::conditional_t<std::is_same_v<StructView, FStructView>,
                                        TStructView<TStructType>, TConstStructView<TStructType>>;

    FTResult Result{};

    static_assert(sizeof(StructView) == sizeof(FTResult));

    // work around constructor issue of TStructView
    FMemory::Memcpy(&Result, &View, sizeof(FTResult));

    return Result;
}

template <CHasStaticStruct TStructType, typename StructView>
    requires (std::is_same_v<StructView, FStructView> || std::is_same_v<StructView, FConstStructView>)
auto TryMakeView(const StructView View)
{
    using FTResult = decltype(MakeView<TStructType>(View));

    if (View.IsValid() && View.GetScriptStruct()->IsChildOf(TStructType::StaticStruct()))
    {
        return MakeView<TStructType>(View);
    }

    return FTResult{};
}

namespace Private
{
template <bool bConstView, typename TStructType, CStructUtils TStructUtils>
auto FindStructViewInternal(TArrayView<TStructUtils> BaseStructsArrayView)
{
    using FResult    = std::conditional_t<bConstView, TConstStructView<TStructType>, TStructView<TStructType>>;
    using StructView = std::conditional_t<bConstView, FConstStructView, FStructView>;

    TTuple<FResult, int32> Result{FResult{}, INDEX_NONE};

    for (auto Index = 0; Index < BaseStructsArrayView.Num(); ++Index)
    {
        auto& BaseStruct = BaseStructsArrayView[Index];

        auto View = TryMakeView<TStructType>(StructView(
            BaseStruct.GetScriptStruct(), const_cast<uint8*>(BaseStruct.GetMemory())));

        if (View.IsValid())
        {
            Result = {View, Index};
            break;
        }
    }

    return Result;
}
}

template <typename TStructType, CStructUtils TStructUtils>
auto FindStructView(TArrayView<TStructUtils> BaseStructsArrayView)
{
    return Private::FindStructViewInternal<false, TStructType>(BaseStructsArrayView);
}

template <typename TStructType, CStructUtils TStructUtils>
auto FindConstStructView(TConstArrayView<TStructUtils> BaseStructsArrayView)
{
    return Private::FindStructViewInternal<true, TStructType>(BaseStructsArrayView);
}

template <typename TStructType, CStructUtils TStructUtils>
void ForEachStructView(TArrayView<TStructUtils> BaseStructsArrayView,
    TFunctionRef<void(TStructType& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef)
{
    for (auto Index = 0; Index < BaseStructsArrayView.Num(); ++Index)
    {
        auto& BaseStruct = BaseStructsArrayView[Index];

        if constexpr (!std::is_const_v<TStructType> &&
                      requires(TStructUtils StructUtils, TStructType* Struct)
                      {
                          // has GetMutablePtr
                          Struct = StructUtils.template GetMutablePtr<TStructType>();
                      })
        {
            if (auto* Ptr = BaseStruct.template GetMutablePtr<TStructType>())
            {
                FunctionRef(*Ptr, Index, *BaseStruct.GetScriptStruct());
            }
        }
        else
        {
            if (auto* Ptr = BaseStruct.template GetPtr<TStructType>())
            {
                FunctionRef(*Ptr, Index, *BaseStruct.GetScriptStruct());
            }
        }
    }
}
}
