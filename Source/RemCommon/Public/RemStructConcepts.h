// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Templates/RemIsInstance.h"

template<typename BaseStructT>
struct TInstancedStruct;

template<typename BaseStructT>
struct TStructView;

template<typename BaseStructT>
struct TConstStructView;

template<typename BaseStructT>
struct TSharedStruct;

template<typename BaseStructT>
struct TConstSharedStruct;

struct FInstancedStruct;
struct FStructView;
struct FConstStructView;
struct FSharedStruct;
struct FConstSharedStruct;

namespace Rem::Concepts
{
	template<typename TStructUtils>
	concept is_struct_utils_template =
				Rem::is_instance_v<TStructUtils, TInstancedStruct>
			|| Rem::is_instance_v<TStructUtils, TStructView>
			|| Rem::is_instance_v<TStructUtils, TConstStructView>
			|| Rem::is_instance_v<TStructUtils, TSharedStruct>
			|| Rem::is_instance_v<TStructUtils, TConstSharedStruct>;

	template<typename TStructUtils>
	concept is_struct_utils_non_template =
			std::is_same_v<std::remove_cvref_t<TStructUtils>, FInstancedStruct>
			|| std::is_same_v<std::remove_cvref_t<TStructUtils>, FStructView>
			|| std::is_same_v<std::remove_cvref_t<TStructUtils>, FConstStructView>
			|| std::is_same_v<std::remove_cvref_t<TStructUtils>, FSharedStruct>
			|| std::is_same_v<std::remove_cvref_t<TStructUtils>, FConstSharedStruct>;

	template<typename TStructUtils>
	concept is_struct_utils = is_struct_utils_non_template<TStructUtils> || is_struct_utils_template<TStructUtils>;
}
