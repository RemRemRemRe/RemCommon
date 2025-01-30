// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "RemAlwaysFalse.h"
#include "Enum/RemComparisonOperator.h"
#include "Templates/RemIsInstance.h"

namespace Rem::Math
{
	constexpr auto CounterClockwiseRotationAngleThreshold{5.0f};

	template<typename T>
	requires (!std::is_signed_v<T>)
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
			static_assert(always_false<T>::value, "T is not supported");
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
	requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T Clamp01(const T Value)
	{
		return FMath::Clamp<T>(Value, 0.0f, 1.0f);
	}
	
	template< class T >
	requires std::is_floating_point_v<T>
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

	template <typename ValueType>
	requires std::is_floating_point_v<ValueType>
	constexpr ValueType RemapAngleForCounterClockwiseRotation(const ValueType Angle)
	{
		if (Angle > 180.0f - CounterClockwiseRotationAngleThreshold)
		{
			return Angle - 360.0f;
		}

		return Angle;
	}

	inline float Damp(const float DeltaTime, const float Smoothing)
	{
		// https://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/

		return 1.0f - FMath::Pow(Smoothing, DeltaTime);
	}

	inline float ExponentialDecay(const float DeltaTime, const float Lambda)
	{
		// https://www.rorydriscoll.com/2016/03/07/frame-rate-independent-damping-using-lerp/

		return 1.0f - FMath::InvExpApprox(Lambda * DeltaTime);
	}

	template<typename ValueType>
	requires std::is_floating_point_v<ValueType>
	struct TAngle
	{
		ValueType Angle;
		explicit constexpr TAngle(const ValueType Angle) : Angle(Angle) {}

		const ValueType& operator*() const
		{
			return Angle;
		}

		ValueType& operator*()
		{
			return Angle;
		}
	};

	template <typename T>
	T Interpolate(const T& From, const T& To, float Ratio)
	{
		if (CompareValue<ERemComparisonOperator::Equals>(From, To))
		{
			return To;
		}

		if constexpr (is_instance_v<T, UE::Math::TQuat>)
		{
			return T::FastLerp(From, To, Ratio).GetNormalized();
		}
		else if constexpr (is_instance_v<T, UE::Math::TRotator>)
		{
			auto Result{To - From};
			Result.Normalize();

			Result.Pitch = RemapAngleForCounterClockwiseRotation(Result.Pitch);
			Result.Yaw = RemapAngleForCounterClockwiseRotation(Result.Yaw);
			Result.Roll = RemapAngleForCounterClockwiseRotation(Result.Roll);

			Result *= Ratio;
			Result += From;
			Result.Normalize();

			return Result;
		}
		else if constexpr (is_instance_v<T, TAngle>)
		{
			auto Delta{FMath::UnwindDegrees(*To - *From)};
			Delta = RemapAngleForCounterClockwiseRotation(*Delta);

			return FMath::UnwindDegrees(*From + *Delta * Ratio);
		}
		else
		{
			return FMath::Lerp(From, To, Ratio);
		}
	}

	template <typename T>
	T ExponentialDecay(const T& Current, const T& Target, const float DeltaTime, const float Lambda)
	{
		if (Lambda <= 0.0f)
		{
			return Target;
		}

		return Interpolate(Current, Target, ExponentialDecay(DeltaTime, Lambda));
	}

	template <typename T>
	T Damp(const T& Current, const T& Target, const float DeltaTime, const float Lambda)
	{
		if (Lambda <= 0.0f)
		{
			return Target;
		}

		return Interpolate(Current, Target, Damp(DeltaTime, Lambda));
	}

	template <typename ValueType, typename StateType>
	ValueType SpringDamp(StateType& SpringState, const ValueType& Current, const ValueType& Target, const float DeltaTime,
		const float Frequency, const float DampingRatio, const float TargetVelocityAmount)
	{
		if (DeltaTime <= UE_SMALL_NUMBER)
		{
			return Current;
		}

		if (!SpringState.bStateValid)
		{
			SpringState.Velocity = ValueType{0.0f};
			SpringState.PreviousTarget = Target;
			SpringState.bStateValid = true;

			return Target;
		}

		ValueType Result{Current};
		FMath::SpringDamper(Result, SpringState.Velocity, Target,
							(Target - SpringState.PreviousTarget) * Clamp01(TargetVelocityAmount) / DeltaTime,
							DeltaTime, Frequency, DampingRatio);

		SpringState.PreviousTarget = Target;

		return Result;
	}

	/**
	 * Combine 2 rotations to give you the resulting rotation of first applying Left, then Right
	 */
	template <typename T>
	[[nodiscard]] auto CombineRotator(const UE::Math::TRotator<T>& Left, const UE::Math::TRotator<T>& Right) -> auto
	{
		return UE::Math::TRotator<T>{Right.Quaternion() * Left.Quaternion()};
	}
}
