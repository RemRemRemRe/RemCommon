//

#pragma once

namespace Common::Math
{
	template<typename T, typename TEnableIf<!TIsSigned<T>::Value && sizeof(T) <= 4U, uint32>::Type X = 1U>
	uint32 GetBitsNeeded(const T Value)
	{
		return 32U - FPlatformMath::CountLeadingZeros(static_cast<uint32>(Value));
	}

	FORCEINLINE FVector MakeVectorFromXY(const FVector Vector)
	{
		return {Vector.X, Vector.Y, 0.f};
	}

	/** Clamps Value to be between 0 and 1, inclusive */
	template< class T >
	UE_NODISCARD static constexpr FORCEINLINE T Clamp01(const T Value)
	{
		return FMath::Clamp<T>(Value, 0, 1);
	}
	
	template< class T >
	UE_NODISCARD static constexpr FORCEINLINE decltype(auto) Saturate(const T Value)
	{
		return Clamp01<T>(Value);
	}

}
