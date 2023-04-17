#pragma once

namespace Rem::Common::Enum::BitOperation
{
	// @see https://wiggling-bits.net/using-enum-classes-as-type-safe-bitmasks/

	template<typename Enum>
	requires std::is_enum_v<Enum>
	struct TEnumClassBitOperationTraits { using type = std::false_type; };
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum constexpr operator& (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) & static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum& operator&= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) &= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum constexpr operator| (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) | static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum& operator|= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) |= static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum constexpr operator^ (const Enum Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^ static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum& operator^= (Enum& Lhs, const Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return Lhs = static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum constexpr operator~ (const Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum>( ~ static_cast<Underlying>(Lhs));
	}

	template<typename Enum>
	requires TEnumClassBitOperationTraits<Enum>::type::value
	Enum constexpr operator! (const Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		const Underlying Result = static_cast<Underlying>(Lhs);
		return Result == 0;
	}	
}
