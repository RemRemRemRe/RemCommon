// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Templates/RemEnumClassBitOperation.h"

#include "RemAngleCombination.generated.h"

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERemAngleCombination : uint8
{
	None	= 0,
	Pitch	= 1 << 0,
	Yaw		= 1 << 1,
	Roll	= 1 << 2,
    
	PitchYaw    = Pitch | Yaw,
	PitchRoll   = Pitch | Roll,
	YawRoll     = Yaw | Roll,
    All         = Pitch | Yaw | Roll,
};

template<>
struct Rem::Enum::BitOperation::TEnumClassBitOperationTraits<ERemAngleCombination> { using type = std::true_type; };
