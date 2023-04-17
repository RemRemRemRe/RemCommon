#pragma once

namespace Rem::Common
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
	using TIsInstance = Private::TIsInstanceImpl<std::_Remove_cvref_t<T>, U>;
	
}
