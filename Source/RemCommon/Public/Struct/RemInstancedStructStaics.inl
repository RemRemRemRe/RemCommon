// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructConcepts.h"
#include "RemConcepts.h"

#include "StructUtils/StructView.h"

namespace Rem::Struct
{
    template <Rem::Concepts::has_static_struct StructType>
    TStructView<StructType> MakeView(const FStructView View)
    {
        using FTResult = TStructView<StructType>;
        FTResult Result{};
        
        static_assert(sizeof(FStructView) == sizeof(FTResult));

        // work around constructor issue of TStructView
        FMemory::Memcpy(&Result, &View, sizeof(FTResult));

        return Result;
    }

    template <Rem::Concepts::has_static_struct StructType>
    TStructView<StructType> TryMakeView(const FStructView View)
    {
        if (View.IsValid() && View.GetScriptStruct()->IsChildOf(StructType::StaticStruct()))
        {
            return MakeView<StructType>(View);
        }

        return TStructView<StructType>{};
    }

    template <Rem::Concepts::has_static_struct StructType>
    TConstStructView<StructType> MakeView(const FConstStructView View)
    {
        using FTResult = TConstStructView<StructType>;
        FTResult Result{};
        
        static_assert(sizeof(FConstStructView) == sizeof(FTResult));

        // work around constructor issue of TStructView
        FMemory::Memcpy(&Result, &View, sizeof(FTResult));

        return Result;
    }

    template <Rem::Concepts::has_static_struct StructType>
    TConstStructView<StructType> TryMakeView(const FConstStructView View)
    {
        if (View.IsValid() && View.GetScriptStruct()->IsChildOf(StructType::StaticStruct()))
        {
            return MakeView<StructType>(View);
        }

        return TConstStructView<StructType>{};
    }
    
	template <typename StructType, Concepts::is_struct_utils TStructUtils>
	auto FindStructView(TArrayView<TStructUtils> BaseStructsArrayView)
	{
		using TResult = TStructView<StructType>;
		TResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
		    Result = TryMakeView<StructType>(FStructView(
		        BaseStruct.GetScriptStruct(), const_cast<uint8*>(BaseStruct.GetMemory())));
		    
			if (Result.IsValid())
			{
			    break;
			}
		}

		return Result;
	}

	template <typename StructType, Concepts::is_struct_utils TStructUtils>
	auto FindConstStructView(TConstArrayView<TStructUtils> BaseStructsArrayView)
	{
		using TResult = TConstStructView<StructType>;
		TResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
		    Result = TryMakeView<StructType>(FConstStructView(
		        BaseStruct.GetScriptStruct(), BaseStruct.GetMemory()));
		    
		    if (Result.IsValid())
		    {
		        break;
		    }
		}

		return Result;
	}

	template <typename StructType, Concepts::is_struct_utils TStructUtils>
	void ForEachStructView(TArrayView<TStructUtils> BaseStructsArrayView, TFunctionRef<void(StructType&)> FunctionRef)
	{
		for (auto& BaseStruct : BaseStructsArrayView)
		{
			if constexpr (!std::is_const_v<StructType> &&
				requires(TStructUtils StructUtils, StructType* Struct)
				{
					// has GetMutablePtr
					Struct = StructUtils.template GetMutablePtr<StructType>();
				})
			{
				if (auto* Ptr = BaseStruct.template GetMutablePtr<StructType>())
				{
					FunctionRef(*Ptr);
				}
			}
			else
			{
				if (auto* Ptr = BaseStruct.template GetPtr<StructType>())
				{
					FunctionRef(*Ptr);
				}
			}
		}
	}
}
