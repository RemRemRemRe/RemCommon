// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Templates/Less.h"
#include "Templates/IdentityFunctor.h"

// copied from Engine/Source/Runtime/Core/Public/Algo/BinarySearch.h
// make RangeValueType a reference rather than pointer to make use of member function overload for []

namespace Rem::Algo
{
namespace Private
{
    
    template <typename RangeValueType, typename SizeType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
    SizeType LowerBound(RangeValueType& Range, const SizeType Num, const PredicateValueType& Value, ProjectionType Projection, SortPredicateType SortPredicate)
    {
        // Current start of sequence to check
        SizeType Start = 0;
        // Size of sequence to check
        SizeType Size = Num;

        // With this method, if Size is even it will do one more comparison than necessary, but because Size can be predicted by the CPU it is faster in practice
        while (Size > 0)
        {
            const SizeType LeftoverSize = Size % 2;
            Size = Size / 2;

            const SizeType CheckIndex = Start + Size;
            const SizeType StartIfLess = CheckIndex + LeftoverSize;

            auto&& CheckValue = Invoke(Projection, Range[CheckIndex]);
            Start = /*!*/SortPredicate(Value, CheckValue) ? StartIfLess : Start;
        }

        return Start;
    }
    
    template <typename RangeValueType, typename SizeType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
    SizeType UpperBound(RangeValueType& Range, const SizeType Num, const PredicateValueType& Value, ProjectionType Projection, SortPredicateType SortPredicate)
    {
        // Current start of sequence to check
        SizeType Start = 0;
        // Size of sequence to check
        SizeType Size = Num;

        // With this method, if Size is even it will do one more comparison than necessary, but because Size can be predicted by the CPU it is faster in practice
        while (Size > 0)
        {
            const SizeType LeftoverSize = Size % 2;
            Size = Size / 2;

            const SizeType CheckIndex = Start + Size;
            const SizeType StartIfLess = CheckIndex + LeftoverSize;

            auto&& CheckValue = Invoke(Projection, Range[CheckIndex]);
            Start = !SortPredicate(Value, CheckValue) ? StartIfLess : Start;
        }

        return Start;
    }
    
}
    
    template <typename RangeValueType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
    [[nodiscard]] UE_REWRITE auto UpperBound(RangeValueType& Range, const PredicateValueType& Value, ProjectionType Projection, SortPredicateType SortPredicate)
    {
        return Private::UpperBound(Range, GetNum(Range), Value, Projection, SortPredicate);
    }

    template <typename RangeValueType, typename PredicateValueType, typename ProjectionType>
    [[nodiscard]] UE_REWRITE auto UpperBound(RangeValueType& Range, const PredicateValueType& Value, ProjectionType Projection)
    {
        return Private::UpperBound(Range, GetNum(Range), Value, Projection, TLess());
    }

    template <typename RangeValueType, typename PredicateValueType>
    [[nodiscard]] UE_REWRITE auto UpperBound(RangeValueType& Range, const PredicateValueType& Value)
    {
        return Private::UpperBound(Range, GetNum(Range), Value, FIdentityFunctor(), TLess());
    }
    
    template <typename RangeValueType, typename PredicateValueType, typename ProjectionType, typename SortPredicateType>
    [[nodiscard]] UE_REWRITE auto LowerBound(RangeValueType& Range, const PredicateValueType& Value, ProjectionType Projection, SortPredicateType SortPredicate)
    {
        return Private::LowerBound(Range, GetNum(Range), Value, Projection, SortPredicate);
    }

    template <typename RangeValueType, typename PredicateValueType, typename ProjectionType>
    [[nodiscard]] UE_REWRITE auto LowerBound(RangeValueType& Range, const PredicateValueType& Value, ProjectionType Projection)
    {
        return Private::LowerBound(Range, GetNum(Range), Value, Projection, TLess());
    }

    template <typename RangeValueType, typename PredicateValueType>
    [[nodiscard]] UE_REWRITE auto LowerBound(RangeValueType& Range, const PredicateValueType& Value)
    {
        return Private::LowerBound(Range, GetNum(Range), Value, FIdentityFunctor(), TLess());
    }

    template <typename RangeType, typename ValueType, typename ProjectionType, typename SortPredicateType>
	[[nodiscard]] auto BinarySearch(const RangeType& Range, const ValueType& Value, ProjectionType Projection, SortPredicateType SortPredicate) -> decltype(GetNum(Range))
	{
		auto CheckIndex = LowerBound(Range, Value, Projection, SortPredicate);
		if (CheckIndex < GetNum(Range))
		{
			auto&& CheckValue = Invoke(Projection, GetData(Range)[CheckIndex]);
			// Since we returned lower bound we already know Value <= CheckValue. So if Value is not < CheckValue, they must be equal
			if (!SortPredicate(Value, CheckValue))
			{
				return CheckIndex;
			}
		}
		return INDEX_NONE;
	}
    
	template <typename RangeType, typename ValueType, typename ProjectionType>
	[[nodiscard]] UE_REWRITE auto BinarySearch(const RangeType& Range, const ValueType& Value, ProjectionType Projection)
	{
		return BinarySearch(Range, Value, Projection, TLess<>());
	}

    template <typename RangeType, typename ValueType>
	[[nodiscard]] UE_REWRITE auto BinarySearch(const RangeType& Range, const ValueType& Value)
	{
		return BinarySearch(Range, Value, FIdentityFunctor(), TLess<>());
	}
}
