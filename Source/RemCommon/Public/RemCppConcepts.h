// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem
{
template <typename T>
concept CEnum = std::is_enum_v<T>;

template <typename T>
concept CScopedEnum = CEnum<T> && !std::is_convertible_v<T, int>;

template <typename T>
concept CUnscopedEnum = CEnum<T> && std::is_convertible_v<T, int>;

template <class T>
concept CHasEquals = requires(bool Result, const T Left, const T Right)
{
    Result = Left.Equals(Right);
};

template <class T>
concept CHasEqualsWithErrorTolerance = requires(bool Result, const T Left, const T Right,
    float ErrorTolerance)
    {
        Result = Left.Equals(Right, ErrorTolerance);
    };
}
