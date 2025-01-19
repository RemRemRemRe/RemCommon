// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

namespace Rem::Enum::BitOperation
{
	// @see https://wiggling-bits.net/using-enum-classes-as-type-safe-bitmasks/

	template<typename Enum>
	requires std::is_enum_v<Enum>
	struct TEnumClassBitOperationTraits { using type = std::false_type; };
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum operator& (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) & static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum& operator&= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) &= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum operator| (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) | static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum& operator|= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) |= static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum operator^ (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^ static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum& operator^= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr Enum operator~ (const Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum>( ~ static_cast<Underlying>(Lhs));
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr bool operator! (const Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		const Underlying Result = static_cast<Underlying>(Lhs);
		return Result == 0;
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr bool HasAllFlags(Enum Flags, Enum Contains)
	{
		using Underlying = std::underlying_type_t<Enum>;
		return (static_cast<Underlying>(Flags) & static_cast<Underlying>(Contains)) == static_cast<Underlying>(Contains);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr bool HasAnyFlags(Enum Flags, Enum Contains)
	{
		using UnderlyingType = std::underlying_type_t<Enum>;
		return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(Contains)) != 0;
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr void AddFlags(Enum& Flags, Enum FlagsToAdd)
	{
		using UnderlyingType = std::underlying_type_t<Enum>;
		Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(FlagsToAdd));
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	constexpr void RemoveFlags(Enum& Flags, Enum FlagsToRemove)
	{
		using UnderlyingType = std::underlying_type_t<Enum>;
		Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) & ~static_cast<UnderlyingType>(FlagsToRemove));
	}
}
