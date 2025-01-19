// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemCardinalDirection.generated.h"

UENUM(BlueprintType)
enum class ERemCardinalDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down,
};

namespace Rem::Enum
{
	template<typename EnumType>
	requires std::is_enum_v<EnumType>
	constexpr EnumType GetAdjacentEnumForPairedEnum(const EnumType Enum)
	{
		using Underlying = std::underlying_type_t<EnumType>;

		const Underlying Value = static_cast<Underlying>(Enum);

		if (Value % 2 == 0)
		{
			return static_cast<EnumType>(Value + 1);
		}
		
		return static_cast<EnumType>(Value - 1);
	}

	constexpr ERemCardinalDirection GetOppositeCardinalDirection(const ERemCardinalDirection Direction)
	{
		return GetAdjacentEnumForPairedEnum(Direction);
	}

	constexpr bool IsDirectionPerpendicular(const ERemCardinalDirection First, const ERemCardinalDirection Second)
	{
		using Underlying = std::underlying_type_t<ERemCardinalDirection>;

		const Underlying ValueFirst = static_cast<Underlying>(First);
		const Underlying ValueSecond = static_cast<Underlying>(Second);

		return FMath::Abs(ValueFirst - ValueSecond) >= 2;
	}
}
