// Copyright RemRemRemRe. 2024. All Rights Reserved.

#pragma once

#include "RemRangeFormat.generated.h"

UENUM(BlueprintType)
enum class ERemRangeFormat : uint8
{
	StartAndEnd,
	StartAndLength,
	LengthAndEnd,
};
