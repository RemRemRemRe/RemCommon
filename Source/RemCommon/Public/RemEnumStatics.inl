// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "Enum/RemHelperEnum.h"
#include "Macro/RemAssertionMacros.h"
#include "Templates/Tuple.h"

namespace Rem::Enum
{

enum class ETransitionType : uint8;

template<typename T>
requires std::is_signed_v<T>
T ApplyTransition(T CurrentValue, TTuple<ETransitionType, T> TransitionData, T End, T Start = 0)
{
#if REM_WITH_DEVELOPMENT_ONLY_CODE

	RemCheckCondition(Start >= 0, return T{INDEX_NONE});
	RemCheckCondition(End > Start, return T{INDEX_NONE});

#endif

	auto&& TransitionTYpe = TransitionData.template Get<0>();
	auto&& Step = TransitionData.template Get<1>();

	switch (TransitionTYpe)
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

}
