// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem::Concepts
{
	template <typename T>
	concept is_scoped_enum = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

	template<class T>
	concept has_equals = requires (bool Result, const T Left, const T Right)
	{
		Result = Left.Equals(Right);
	};

	template<class T>
	concept has_equals_with_error_tolerance = requires (bool Result, const T Left, const T Right,
		float ErrorTolerance)
	{
		Result = Left.Equals(Right, ErrorTolerance);
	};
}
