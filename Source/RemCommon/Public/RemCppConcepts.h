// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem::Concepts
{
	template <typename T>
	concept is_scoped_enum = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;
}
