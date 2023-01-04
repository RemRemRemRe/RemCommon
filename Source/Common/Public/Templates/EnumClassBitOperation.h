#pragma once

namespace Common::Enum::BitOperation
{
	// @see https://wiggling-bits.net/using-enum-classes-as-type-safe-bitmasks/
	
	template<typename Enum>
	struct TEnumClassBitOperationTraitsBase { using Result = std::false_type; };

	template<typename Enum>
	struct TEnumClassBitOperationTraits : TEnumClassBitOperationTraitsBase<Enum>
	{ };
	
	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator& (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) & static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator&= (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) &= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator| (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) | static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator|= (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;

		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) |= static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator^ (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^ static_cast<Underlying>(Rhs)
		);
	}
	
	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator^= (Enum Lhs, Enum Rhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum> (
			static_cast<Underlying>(Lhs) ^= static_cast<Underlying>(Rhs)
		);
	}

	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, Enum>::
	Type constexpr operator~ (Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		return static_cast<Enum>( ~ static_cast<Underlying>(Lhs));
	}

	template<typename Enum>
	typename TEnableIf<std::conjunction_v<
				std::is_enum<Enum>,
				typename TEnumClassBitOperationTraits<Enum>::Result
			>, bool>::
	Type constexpr operator! (Enum Lhs) noexcept
	{
		using Underlying = std::underlying_type_t<Enum>;
		
		const Underlying Result = static_cast<Underlying>(Lhs);
		return Result == 0;
	}	
}
