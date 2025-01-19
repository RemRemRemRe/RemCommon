// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Templates/RemEnumClassBitOperation.h"

#include "RemAxisCombination.generated.h"

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERemAxisCombination : uint8
{
	None	= 0,
	X		= 1 << 0,
	Y		= 1 << 1,
	Z		= 1 << 2,
};

template<>
struct Rem::Enum::BitOperation::TEnumClassBitOperationTraits<ERemAxisCombination> { using type = std::true_type;	};
