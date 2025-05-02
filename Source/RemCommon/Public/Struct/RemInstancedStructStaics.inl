// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructConcepts.h"
#include "StructUtils/StructView.h"

namespace Rem::Struct
{
	template <typename StructType, Concepts::is_struct_utils TStructUtils>
	auto FindStructView(TArrayView<TStructUtils> BaseStructsArrayView)
	{
		using TResult = TStructView<StructType>;
		TResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
			if (BaseStruct.template GetPtr<const StructType>())
			{
				const FConstStructView Intermediate{BaseStruct.GetScriptStruct(), BaseStruct.GetMemory()};

				static_assert(sizeof(FConstStructView) == sizeof(TResult));

				// work around constructor issue of TStructView
				FMemory::Memcpy(&Result, &Intermediate, sizeof(FConstStructView));
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
			if (BaseStruct.template GetPtr<const StructType>())
			{
				const FConstStructView Intermediate{BaseStruct.GetScriptStruct(), BaseStruct.GetMemory()};

				static_assert(sizeof(FConstStructView) == sizeof(TResult));

				// work around constructor issue of TStructView
				FMemory::Memcpy(&Result, &Intermediate, sizeof(FConstStructView));
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
