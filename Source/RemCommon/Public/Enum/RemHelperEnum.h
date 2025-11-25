// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"

//// strong type enum for replacing boolean usages in places like template

namespace Rem::Enum
{
	enum class ENewOrOld : uint8
	{
		New,
		Old,
	};

	enum class EBreakMode : uint8
	{
		BreakOnFirstElement,
		NoBreak,
	};

	enum class EOptionalMode : uint8
	{
		Optional,
		Required,
	};

	enum class EHierarchyMatchMode : uint8
	{
		IncludeChildren,
		MatchExact,
	};

	enum class EAttributeValueType : uint8
	{
		Base,
		Current,
	};

	enum class EYesOrNo : uint8
	{
		Yes,
		No,
	};

    enum class EStartOrEnd : uint8
	{
		Start,
		End,
	};

	enum class ETransitionType : uint8
	{
		ToNext,
		ToPrevious,
		ToSpecified,
	};
}
