// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

// Shared primitives for type-directed lookup over struct-view-like collections:
// typed view construction (MakeView / TryMakeView) and the generic range walk
// (FindStructViewInternal). Consumed by RemInstancedStructStatics.inl (TArrayView
// overloads) and RemScopedStructContainer.h (container overloads); include this
// file directly only when building on top of these primitives.

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
template <bool bConstView, typename TStructType, typename TRange>
auto FindStructViewInternal(const TRange& Range)
    requires (CStructUtils<std::remove_cvref_t<decltype(*Range.begin())>>)
{
    using FResultView = std::conditional_t<bConstView, TConstStructView<TStructType>, TStructView<TStructType>>;
    using StructView  = std::conditional_t<bConstView, FConstStructView, FStructView>;

    using FResult = TTuple<FResultView, int32>;

    int32 Index = 0;
    for (const auto& BaseStruct : Range)
    {
        auto View = TryMakeView<TStructType>(StructView(
            BaseStruct.GetScriptStruct(), const_cast<uint8*>(BaseStruct.GetMemory())));

        if (View.IsValid())
        {
            return FResult{View, Index};
        }
        ++Index;
    }

    return FResult{FResultView{}, INDEX_NONE};
}
}
}
