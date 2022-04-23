#pragma once


namespace Common
{
	namespace Impl
	{
		template <typename, template <typename...> typename>
		struct TIsInstanceImpl : std::false_type {};

		template <template <typename...> typename U, typename...Ts>
		struct TIsInstanceImpl<U<Ts...>, U> : std::true_type {};
	}

	// check if type T is an instantiation of template U
	// @see https://stackoverflow.com/a/61040973
	template <typename T, template <typename ...> typename U>
	using TIsInstance = Impl::TIsInstanceImpl<std::decay_t<T>, U>;
	
}
