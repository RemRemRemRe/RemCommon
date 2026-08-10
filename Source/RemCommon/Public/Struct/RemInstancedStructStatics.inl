// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Struct/RemStructViewStatics.inl"

namespace Rem::Struct
{
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
