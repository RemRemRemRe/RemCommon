// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemComparisonOperatorType.generated.h"

UENUM(BlueprintType)
enum class ERemComparisonOperatorType : uint8
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
	template<ERemComparisonOperatorType ComparisonOperator, float ErrorTolerance = UE_SMALL_NUMBER >
	bool CheckFloatWithThreshold(const float Value, const float Threshold)
	{
		if constexpr (ComparisonOperator == ERemComparisonOperatorType::Equals)
		{
			return FMath::IsNearlyEqual(Value, Threshold, ErrorTolerance);
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperatorType::NotEqual)
		{
			return !FMath::IsNearlyEqual(Value, Threshold, ErrorTolerance);
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperatorType::LessThan)
		{
			return Value < Threshold;
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperatorType::LessThanOrEqual)
		{
			return Value <= Threshold;
		}
		else if constexpr (ComparisonOperator == ERemComparisonOperatorType::GreaterThan)
		{
			return Value > Threshold;
		}
		else// if constexpr (ComparisonOperator == ERemEqualityOperatorType::GreaterThanOrEqual)
		{
			return Value >= Threshold;
		}
	}
}
