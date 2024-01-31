// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#define REM_ON_SCOPE_EXIT_WEAK auto PREPROCESSOR_JOIN(_weakScopeExit_, __LINE__) = \
	Rem::FHelper(this) += [&]

#define REM_ON_SCOPE_EXIT_WEAK_OBJECT(Object) \
	auto PREPROCESSOR_JOIN(_weakScopeExit_, __LINE__) = \
	Rem::FHelper(Object) += [&]

namespace Rem
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

	struct FHelper
	{
		const UObject* Object;
		explicit FHelper(const UObject* Object) : Object(Object) { }
		template<typename F>
		TWeakScopeExit<F> operator+=(F Callback)
		{
			return TWeakScopeExit(Object, MoveTemp(Callback));
		}
	};
}
