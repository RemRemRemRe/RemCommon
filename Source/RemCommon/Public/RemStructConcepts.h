// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Templates/RemInstanceOf.h"

template <typename BaseStructT>
struct TInstancedStruct;

template <typename BaseStructT>
struct TStructView;

template <typename BaseStructT>
struct TConstStructView;

template <typename BaseStructT>
struct TSharedStruct;

template <typename BaseStructT>
struct TConstSharedStruct;

struct FInstancedStruct;
struct FStructView;
struct FConstStructView;
struct FSharedStruct;
struct FConstSharedStruct;

namespace Rem
{
template <typename TStructUtils>
concept CTStructUtils =
    Rem::CInstanceOf<TStructUtils, TInstancedStruct>
    || Rem::CInstanceOf<TStructUtils, TStructView>
    || Rem::CInstanceOf<TStructUtils, TConstStructView>
    || Rem::CInstanceOf<TStructUtils, TSharedStruct>
    || Rem::CInstanceOf<TStructUtils, TConstSharedStruct>;

template <typename TStructUtils>
concept CFStructUtils =
    std::is_same_v<std::remove_cvref_t<TStructUtils>, FInstancedStruct>
    || std::is_same_v<std::remove_cvref_t<TStructUtils>, FStructView>
    || std::is_same_v<std::remove_cvref_t<TStructUtils>, FConstStructView>
    || std::is_same_v<std::remove_cvref_t<TStructUtils>, FSharedStruct>
    || std::is_same_v<std::remove_cvref_t<TStructUtils>, FConstSharedStruct>;

template <typename TStructUtils>
concept CStructUtils = CFStructUtils<TStructUtils> || CTStructUtils<TStructUtils>;
}
