// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemAssertionMacros.h"

#include "RemArrayOperation.generated.h"

UENUM(BlueprintType)
enum class ERemArrayOperation : uint8
{
	/**
	 * try put elements in the end of the array
	 */
	PushBack,

	/**
	 * try put elements in the front of the array
	 */
	InsertFront,
};

namespace Rem::Object
{
	template<ERemArrayOperation Operation, typename T>
	void ApplyArrayOperation(TArray<T>& OutArray, const typename TArray<T>::ElementType& ReferenceElementOfOutArray,
		TConstArrayView<typename TArray<T>::ElementType> InArray)
	{
		RemCheckCondition(!InArray.IsEmpty(), return;);

		if (const auto Index = OutArray.Find(ReferenceElementOfOutArray);
			Index != INDEX_NONE)
		{
			if constexpr (Operation == ERemArrayOperation::InsertFront)
			{
				// in the front of reference asset
				OutArray.Insert(InArray.GetData(), InArray.Num(), Index);
			}
			else
			{
				// in the back of reference asset
				const auto ReferenceIndexPlusOne = Index + 1;
				if (ReferenceIndexPlusOne <= OutArray.Num() - 1)
				{
					OutArray.Insert(InArray.GetData(), InArray.Num(), ReferenceIndexPlusOne);
				}
				else
				{
					OutArray.Append(InArray);
				}
			}
		}
		else
		{
			if constexpr (Operation == ERemArrayOperation::InsertFront)
			{
				// in the front
				if (!OutArray.IsEmpty())
				{
					OutArray.Insert(InArray.GetData(), InArray.Num(), 0);
				}
				else
				{
					OutArray.Append(InArray);
				}
			}
			else
			{
				// in the back
				OutArray.Append(InArray);
			}
		}
	}

	/**
	 * InsertFront/PushBack the InArray at the index of ReferenceElementOfOutArray to OutArray
	 *
	 * if index couldn't be found, InArray will be added to front or end of OutArray respectively
	 */
	template<typename T>
	void ApplyArrayOperation(TArray<T>& OutArray, const typename TArray<T>::ElementType& ReferenceElementOfOutArray,
		const ERemArrayOperation Operation, TConstArrayView<typename TArray<T>::ElementType> InArray)
	{
		if (Operation == ERemArrayOperation::InsertFront)
		{
			return ApplyArrayOperation<ERemArrayOperation::InsertFront, T>(OutArray, ReferenceElementOfOutArray, InArray);
		}
		return ApplyArrayOperation<ERemArrayOperation::PushBack, T>(OutArray, ReferenceElementOfOutArray, InArray);
	}
}
