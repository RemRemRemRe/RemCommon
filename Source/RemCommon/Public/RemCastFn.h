// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Templates/Casts.h"

namespace Rem
{

/**
 * Functor wrapper for Cast<T> template of UE.
 *
 * Pass to transrangers::transform as a callable without overload ambiguity.
 * Supports all types that Cast<T> supports: raw pointers, TObjectPtr, TWeakObjectPtr, etc.
 *
 * Usage:
 *   transrangers::transform(Rem::Cast<URemTagEventSourceTriggerSection>, ranger)
 *
 * Why not REM_FUNCTION_TO_FUNCTOR? Cast<T> has multiple overloads; a functor struct
 * defers template deduction to call site where argument types are known.
 */
template <typename To>
struct TCast
{
	template <typename From>
	[[nodiscard]] constexpr decltype(auto) operator()(From&& Ptr) const
	{
		return Cast<To>(std::forward<From>(Ptr));
	}
};

/**
 * Convenience constexpr variable template. Use Rem::Cast<SomeType> directly in pipelines.
 */
template <typename To>
inline constexpr TCast<To> Cast{};

}
