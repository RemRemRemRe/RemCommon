// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

namespace Rem::Concepts
{

template <typename T>
concept is_enum_class = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

}