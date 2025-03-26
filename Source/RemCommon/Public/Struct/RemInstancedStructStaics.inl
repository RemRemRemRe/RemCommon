// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemStructConcepts.h"
#include "StructUtils/StructView.h"

namespace Rem::Struct
{
	template <typename StructType, typename TStructUtils>
	requires Concepts::is_struct_utils<TStructUtils>
	auto GetStructView(TArrayView<TStructUtils> BaseStructsArrayView)
	{
		using TResult = TStructView<StructType>;
		TResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
			if (auto* Ptr = BaseStruct.template GetPtr<StructType>())
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

	template <typename StructType, typename TStructUtils>
	requires Concepts::is_struct_utils<TStructUtils>
	auto GetConstStructView(TConstArrayView<TStructUtils> BaseStructsArrayView)
	{
		using TResult = TConstStructView<StructType>;
		TResult Result{};

		for (auto& BaseStruct : BaseStructsArrayView)
		{
			if (auto* Ptr = BaseStruct.template GetPtr<const StructType>())
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

	template <typename StructType, typename TStructUtils>
	requires Concepts::is_struct_utils<TStructUtils>
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
