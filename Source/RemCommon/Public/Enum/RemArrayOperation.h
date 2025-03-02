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
	/**
	 * InsertFront/PushBack the InArray at the index of ReferenceElementOfOutArray to OutArray
	 *
	 * if index couldn't be found, InArray will be added to front or end of OutArray respectively
	 */
	template<typename T>
	void ApplyArrayOperation(TArray<T>& OutArray, const T& ReferenceElementOfOutArray, const ERemArrayOperation Operation, const TArray<T>& InArray)
	{
		RemCheckCondition(!InArray.IsEmpty(), return;);

		if (const auto Index = OutArray.Find(ReferenceElementOfOutArray);
			Index != INDEX_NONE)
		{
			if (Operation == ERemArrayOperation::InsertFront)
			{
				// in the front of reference asset
				OutArray.Insert(InArray, Index);
			}
			else
			{
				// in the back of reference asset
				OutArray.Insert(InArray, Index + 1);
			}
		}
		else
		{
			if (Operation == ERemArrayOperation::InsertFront)
			{
				// in the front
				OutArray.Insert(InArray, 0);
			}
			else
			{
				// in the back
				OutArray.Append(InArray);
			}
		}
	}

	template<ERemArrayOperation Operation, typename T>
	void ApplyArrayOperation(TArray<T>& OutArray, const T& ReferenceElementOfOutArray, const TArray<T>& InArray)
	{
		return ApplyArrayOperation(OutArray, ReferenceElementOfOutArray, Operation, InArray);
	}
}
