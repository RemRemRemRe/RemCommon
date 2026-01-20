// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Enum/RemHelperEnum.h"
#include "Macro/RemAssertionMacros.h"

namespace Rem::Enum
{

enum class ETransitionType : uint8;

template<typename T>
requires std::is_signed_v<T>
[[nodiscard]] T ApplyTransition(T CurrentValue, const ETransitionType TransitionType, T Step, T End, T Start = 0)
{
#if REM_WITH_DEVELOPMENT_ONLY_CODE

	RemCheckCondition(Start >= 0, return T{INDEX_NONE});
	RemCheckCondition(End > Start, return T{INDEX_NONE});

#endif

	switch (TransitionType)
	{
	default:
	case ETransitionType::ToNext:
		if (Step >= 0)
		{
			CurrentValue = FMath::Min(End, CurrentValue + Step);
		}
		else
		{
			// special case : move to end
			CurrentValue = End - 1;
		}
		break;
	case ETransitionType::ToPrevious:
		if (Step >= 0)
		{
			CurrentValue = FMath::Max(Start, CurrentValue - Step);
		}
		else
		{
			// special case : move to start
			CurrentValue = Start;
		}
		break;
	case ETransitionType::ToSpecified:
		CurrentValue = Step;
		break;
	}

	return CurrentValue;
}

template<typename T>
requires std::is_signed_v<T>
[[nodiscard]] T ApplyTransition(T CurrentValue, TTuple<ETransitionType, T> TransitionData, T End, T Start = 0)
{
	return ApplyTransition(CurrentValue, TransitionData.template Get<ETransitionType>(),
	    TransitionData.template Get<T>(), End, Start);
}

namespace Transition
{

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> Abort = {ETransitionType::ToSpecified, -1};

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> ToNext = {ETransitionType::ToNext, 1};

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> ToPrevious = {ETransitionType::ToPrevious, 1};

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> ToStart = {ETransitionType::ToPrevious, -1};

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> ToEnd = {ETransitionType::ToNext, -1};

template<typename T>
requires std::is_signed_v<T>
constexpr TTuple<ETransitionType, T> Restart = {ETransitionType::ToNext, 0};

}

}
