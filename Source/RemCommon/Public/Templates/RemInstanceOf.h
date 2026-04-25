// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem
{

namespace Private
{

// check if type T is an instantiation of template U
// @see https://stackoverflow.com/a/61040973
template <typename, template <typename...> typename>
struct TInstanceOfImpl : std::false_type
{
};

template <template <typename...> typename U, typename... Ts>
struct TInstanceOfImpl<const volatile U<Ts...>, U> : std::true_type
{
};

// @see Engine/Source/Runtime/Core/Public/Concepts/ConceptDefinitionRules.md
template <typename T, template <typename...> typename U>
concept CInstanceOfPrivate = TInstanceOfImpl<T, U>::value;

}

template <typename T, template <typename...> typename U>
concept CInstanceOf = Private::CInstanceOfPrivate<const volatile std::remove_reference_t<T>, U>;

}
