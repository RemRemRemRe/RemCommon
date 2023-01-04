// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CardinalDirection.generated.h"

UENUM(BlueprintType)
enum class ECardinalDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down,
};

namespace Common::Enum
{
	template<typename EnumType>
	constexpr EnumType GetAdjacentEnumForPairedEnum(const EnumType Enum)
	{
		static_assert(std::is_enum_v<EnumType>, "The type passed in is not a enum type");
		
		using Underlying = std::underlying_type_t<ECardinalDirection>;

		const Underlying Value = static_cast<Underlying>(Enum);

		if (Value % 2 == 0)
		{
			return static_cast<EnumType>(Value + 1);
		}
		
		return static_cast<EnumType>(Value - 1);
	}

	constexpr ECardinalDirection GetOppositeCardinalDirection(const ECardinalDirection Direction)
	{
		return GetAdjacentEnumForPairedEnum(Direction);
	}

	constexpr bool IsDirectionPerpendicular(const ECardinalDirection First, const ECardinalDirection Second)
	{
		using Underlying = std::underlying_type_t<ECardinalDirection>;

		const Underlying ValueFirst = static_cast<Underlying>(First);
		const Underlying ValueSecond = static_cast<Underlying>(Second);

		return FMath::Abs(ValueFirst - ValueSecond) >= 2;
	}
}
