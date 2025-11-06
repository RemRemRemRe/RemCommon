// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructConcepts.h"
#include "RemConcepts.h"

#include "StructUtils/StructView.h"

namespace Rem::Struct
{
    template <Rem::Concepts::has_static_struct TStructType>
    TStructView<TStructType> MakeView(const FStructView View)
    {
        using FTResult = TStructView<TStructType>;
        FTResult Result{};
        
        static_assert(sizeof(FStructView) == sizeof(FTResult));

        // work around constructor issue of TStructView
        FMemory::Memcpy(&Result, &View, sizeof(FTResult));

        return Result;
    }

    template <Rem::Concepts::has_static_struct TStructType>
    TStructView<TStructType> TryMakeView(const FStructView View)
    {
        if (View.IsValid() && View.GetScriptStruct()->IsChildOf(TStructType::StaticStruct()))
        {
            return MakeView<TStructType>(View);
        }

        return TStructView<TStructType>{};
    }

    template <Rem::Concepts::has_static_struct TStructType>
    TConstStructView<TStructType> MakeView(const FConstStructView View)
    {
        using FTResult = TConstStructView<TStructType>;
        FTResult Result{};
        
        static_assert(sizeof(FConstStructView) == sizeof(FTResult));

        // work around constructor issue of TStructView
        FMemory::Memcpy(&Result, &View, sizeof(FTResult));

        return Result;
    }

    template <Rem::Concepts::has_static_struct TStructType>
    TConstStructView<TStructType> TryMakeView(const FConstStructView View)
    {
        if (View.IsValid() && View.GetScriptStruct()->IsChildOf(TStructType::StaticStruct()))
        {
            return MakeView<TStructType>(View);
        }

        return TConstStructView<TStructType>{};
    }
    
	template <typename TStructType, Concepts::is_struct_utils TStructUtils>
	auto FindStructView(TArrayView<TStructUtils> BaseStructsArrayView)
	{
		using FResult = TStructView<TStructType>;
		FResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
		    Result = TryMakeView<TStructType>(FStructView(
		        BaseStruct.GetScriptStruct(), const_cast<uint8*>(BaseStruct.GetMemory())));
		    
			if (Result.IsValid())
			{
			    break;
			}
		}

		return Result;
	}

	template <typename TStructType, Concepts::is_struct_utils TStructUtils>
	auto FindConstStructView(TConstArrayView<TStructUtils> BaseStructsArrayView)
	{
		using FResult = TConstStructView<TStructType>;
		FResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
		    Result = TryMakeView<TStructType>(FConstStructView(
		        BaseStruct.GetScriptStruct(), BaseStruct.GetMemory()));
		    
		    if (Result.IsValid())
		    {
		        break;
		    }
		}

		return Result;
	}

	template <typename TStructType, Concepts::is_struct_utils TStructUtils>
	void ForEachStructView(TArrayView<TStructUtils> BaseStructsArrayView, TFunctionRef<void(TStructType&)> FunctionRef)
	{
		for (auto& BaseStruct : BaseStructsArrayView)
		{
			if constexpr (!std::is_const_v<TStructType> &&
				requires(TStructUtils StructUtils, TStructType* Struct)
				{
					// has GetMutablePtr
					Struct = StructUtils.template GetMutablePtr<TStructType>();
				})
			{
				if (auto* Ptr = BaseStruct.template GetMutablePtr<TStructType>())
				{
					FunctionRef(*Ptr);
				}
			}
			else
			{
				if (auto* Ptr = BaseStruct.template GetPtr<TStructType>())
				{
					FunctionRef(*Ptr);
				}
			}
		}
	}
}
