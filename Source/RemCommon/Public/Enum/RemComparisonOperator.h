// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemCppConcepts.h"

#include "RemComparisonOperator.generated.h"

UENUM(BlueprintType)
enum class ERemComparisonOperator : uint8
{
	Equals,
	NotEqual,
	LessThan,
	LessThanOrEqual,
	GreaterThan,
	GreaterThanOrEqual,
};

namespace Rem::Math
{
	template<ERemComparisonOperator ComparisonOperator, float ErrorTolerance = UE_SMALL_NUMBER, typename T = float>
	bool CompareValue(const T& Left, const T& Right)
	{
		if constexpr (ComparisonOperator == ERemComparisonOperator::Equals)
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				return FMath::IsNearlyEqual(Left, Right, ErrorTolerance);
			}
			else if constexpr (Concepts::has_equals<T>)
			{
				return Left.Equals(Right, ErrorTolerance);
			}
			else
			{
				return Left == Right;
			}
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperator::NotEqual)
		{
			return !CompareValue<ERemComparisonOperator::Equals, ErrorTolerance, T>(Left, Right);
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperator::LessThan)
		{
			return Left < Right;
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperator::LessThanOrEqual)
		{
			return Left <= Right;
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperator::GreaterThan)
		{
			return Left > Right;
		}
		else// if constexpr (ComparisonOperator == ERemEqualityOperatorType::GreaterThanOrEqual)
		{
			return Left >= Right;
		}
	}
}
