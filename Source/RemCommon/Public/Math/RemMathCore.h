// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Math/UnrealMathUtility.h"

namespace Rem::Math
{
    constexpr auto CounterClockwiseRotationAngleThreshold{5.0f};
    
    constexpr auto FiveDigitsAfterDecimalPoint{1.e-5f}; // Five digits after the decimal point
    constexpr auto FiveDigitsAfterDecimalPointDouble{1.e-5}; // Five digits after the decimal point
    
    template<typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr auto&& GetKindSmallNumber()
    {
        if constexpr (std::is_same_v<float, T>)
        {
            return UE_KINDA_SMALL_NUMBER;
        }
        else
        {
            static_assert(std::is_same_v<double, T>);
            return UE_DOUBLE_KINDA_SMALL_NUMBER;
        }
    }

    template<typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr auto&& GetSmallNumber()
    {
        if constexpr (std::is_same_v<float, T>)
        {
            return UE_SMALL_NUMBER;
        }
        else
        {
            static_assert(std::is_same_v<double, T>);
            return UE_DOUBLE_SMALL_NUMBER;
        }
    }
    
    template <typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool IsNearlyEqual(T Left, T Right, T Tolerance = GetKindSmallNumber<T>())
    {
        return FMath::IsNearlyEqual(Left, Right, Tolerance);
    }

    template <typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool IsNearlyZero(T Value, T Tolerance = GetKindSmallNumber<T>())
    {
        return FMath::IsNearlyZero(Value, Tolerance);
    }
}
