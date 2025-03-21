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
				static_assert(sizeof(BaseStruct) == sizeof(TResult));

				// work around constructor issue of TStructView
				FMemory::Memcpy(&Result, &BaseStruct, sizeof(TResult));
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
			if (auto* Ptr = BaseStruct.template GetPtr<StructType>())
			{
				static_assert(sizeof(BaseStruct) == sizeof(TResult));

				// work around constructor issue of TConstStructView
				FMemory::Memcpy(&Result, &BaseStruct, sizeof(TResult));
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
			if (auto* Ptr = BaseStruct.template GetPtr<StructType>())
			{
				FunctionRef(*Ptr);
			}
		}
	}
}
