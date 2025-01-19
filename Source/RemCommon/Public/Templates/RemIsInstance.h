﻿// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem
{
	namespace Private
	{
		template <typename, template <typename...> typename>
		struct TIsInstanceImpl : std::false_type {};

		template <template <typename...> typename U, typename...Ts>
		struct TIsInstanceImpl<U<Ts...>, U> : std::true_type {};
	}

	// check if type T is an instantiation of template U
	// @see https://stackoverflow.com/a/61040973
	template <typename T, template <typename ...> typename U>
	using is_instance = Private::TIsInstanceImpl<std::remove_cvref_t<T>, U>;

	template <typename T, template <typename ...> typename U>
	constexpr bool is_instance_v = Private::TIsInstanceImpl<std::remove_cvref_t<T>, U>::value;
}
