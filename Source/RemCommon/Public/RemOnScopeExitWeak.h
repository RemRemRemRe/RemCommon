// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "UObject/WeakObjectPtr.h"

#define REM_ON_SCOPE_EXIT_WEAK auto PREPROCESSOR_JOIN(_weakScopeExit_, __LINE__) = \
	Rem::ScopeExit::FOperatorHelper(this) += [&]

#define REM_ON_SCOPE_EXIT_WEAK_OBJECT(Object) \
	auto PREPROCESSOR_JOIN(_weakScopeExit_, __LINE__) = \
	Rem::ScopeExit::FOperatorHelper(Object) += [&]

class UObject;

namespace Rem::ScopeExit
{
	template<typename F>
	struct TWeakScopeExit
	{
		FWeakObjectPtr Weak;
		F Finally;

		explicit TWeakScopeExit(const UObject* Object, F&& Callback)
			: Weak(Object), Finally(MoveTemp(Callback)) { }

		~TWeakScopeExit() { if (Weak.IsValid()) Finally(); }
	};

	struct FOperatorHelper
	{
		const UObject* Object;
		explicit FOperatorHelper(const UObject* Object) : Object(Object) { }
		template<typename F>
		TWeakScopeExit<F> operator+=(F Callback)
		{
			return TWeakScopeExit(Object, MoveTemp(Callback));
		}
	};
}
