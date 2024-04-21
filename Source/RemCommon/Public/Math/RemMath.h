// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

namespace Rem::Math
{
	template<typename T>
	requires !std::is_signed_v<T>
	T GetBitsNeeded(const T Value)
	{
		if constexpr (std::is_same_v<T, uint8>)
		{
			return std::numeric_limits<T>::max() - FPlatformMath::CountLeadingZeros8(Value);
		}
		else if constexpr (std::is_same_v<T, uint32>)
		{
			return std::numeric_limits<T>::max() - FPlatformMath::CountLeadingZeros(Value);
		}
		else if constexpr (std::is_same_v<T, uint16>)
		{
			return static_cast<T>(std::numeric_limits<T>::max() - FPlatformMath::CountLeadingZeros(Value));
		}
		else if constexpr (std::is_same_v<T, uint64>)
		{
			return std::numeric_limits<T>::max() - FPlatformMath::CountLeadingZeros64(Value);
		}
		else
		{
			static_assert(std::_Always_false<T>, "T is not supported");
			return {};
		}
	}

	template<typename T>
	UE::Math::TVector<T> MakeVectorFromXY(const UE::Math::TVector<T>& Vector)
	{
		return {Vector.X, Vector.Y, 0.f};
	}

	/** Clamps Value to be between 0 and 1, inclusive */
	template< class T >
	[[nodiscard]] constexpr T Clamp01(const T Value)
	{
		return FMath::Clamp<T>(Value, 0.0f, 1.0f);
	}
	
	template< class T >
	[[nodiscard]] constexpr decltype(auto) Saturate(const T Value)
	{
		return Clamp01<T>(Value);
	}

	template<class T>
	auto CombineNumericRanges(const UE::Math::TVector2<T>& Range1, const UE::Math::TVector2<T>& Range2) -> decltype(auto)
	{
		return CombineNumericRanges(Range1.X, Range2.X, Range1.Y, Range2.Y);
	}

	template<class T>
	auto CombineNumericRanges(const T& Min1, const T& Min2, const T& Max1, const T& Max2) -> decltype(auto)
	{
		return UE::Math::TVector2<T>{std::max(Min1, Min2), std::min(Max1, Max2) };
	}

}
