// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Misc/NotNull.h"
#include "Macro/RemFunctorMacro.h"

namespace Rem
{
template <typename T>
using TNotNull = TNotNull<T>;

template <typename T>
concept CNotNull = UE::Core::Private::TIsTNotNullParam_V<T>;

template <typename NotNull, typename ValueTypeBase>
concept CNotNullOf = CNotNull<NotNull>
                     && std::derived_from<std::remove_cvref_t<std::remove_pointer_t<decltype(*std::declval<
                                              NotNull>())>>, ValueTypeBase>;

/**
 * pointer can convert / construct TNotNull implicitly, but there are cases to copy const from T
 */
template <typename T>
decltype(auto) MakeNotNull(T* Object)
{
    return TNotNull<T*>{Object};
}

template <typename T>
decltype(auto) MakeNotNull(TObjectPtr<T> Object)
{
    return MakeNotNull(Object.Get());
}

REM_FUNCTION_TO_FUNCTOR_SIMPLE(MakeNotNull)
}
