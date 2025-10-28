// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

#include "RemAlwaysFalse.h"
#include "Enum/RemComparisonOperator.h"
#include "Templates/RemIsInstance.h"

namespace Rem::Math
{
	constexpr auto CounterClockwiseRotationAngleThreshold{5.0f};
    
	constexpr auto FiveDigitsAfterDecimalPoint{1.e-5f}; // Five digits after the decimal point

	template<typename T>
	requires (!std::is_signed_v<T>)
	[[nodiscard]] constexpr T GetBitsNeeded(const T Value)
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
	[[nodiscard]] constexpr UE::Math::TVector<T> MakeVectorFromXY(const UE::Math::TVector<T>& Vector)
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
	[[nodiscard]] constexpr auto CombineNumericRanges(const UE::Math::TVector2<T>& Range1, const UE::Math::TVector2<T>& Range2)
	{
		return CombineNumericRanges(Range1.X, Range2.X, Range1.Y, Range2.Y);
	}

	template<class T>
	[[nodiscard]] constexpr auto CombineNumericRanges(const T& Min1, const T& Min2, const T& Max1, const T& Max2)
	{
		return UE::Math::TVector2<T>{std::max(Min1, Min2), std::min(Max1, Max2) };
	}

	template <typename T>
	requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr T RemapAngleForCounterClockwiseRotation(const T Angle)
	{
		if (Angle > 180.0f - CounterClockwiseRotationAngleThreshold)
		{
			return Angle - 360.0f;
		}

		return Angle;
	}

	[[nodiscard]] constexpr float DamperExact(const float DeltaTime, const float HalfLife)
	{
		// https://theorangeduck.com/page/spring-roll-call#exactdamper

		return 1.0f - FMath::InvExpApprox(UE_LN2 / (HalfLife + FiveDigitsAfterDecimalPoint) * DeltaTime);
	}
    
    template<typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr auto&& GetKindSmallNumber()
    {
        if constexpr (std::is_same_v<float, T>)
        {
            return GlobalVectorConstants::KindaSmallNumber;
        }
        else
        {
            static_assert(std::is_same_v<double, T>);
            return GlobalVectorConstants::DoubleKindaSmallNumber;
        }
    }

    template<typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr auto&& GetSmallNumber()
    {
        if constexpr (std::is_same_v<float, T>)
        {
            return GlobalVectorConstants::SmallNumber;
        }
        else
        {
            static_assert(std::is_same_v<double, T>);
            return GlobalVectorConstants::DoubleSmallNumber;
        }
    }

	template<typename TValue>
	requires std::is_floating_point_v<TValue>
	struct TAngle
	{
	    using FReal = TValue;
	    
		TValue Angle;
		explicit constexpr TAngle(const TValue Angle) : Angle(Angle) {}

		const TValue& operator*() const
		{
			return Angle;
		}

		TValue& operator*()
		{
			return Angle;
		}

		bool Equals(const TAngle& V, TValue Tolerance = GetKindSmallNumber<TValue>()) const
		{
			return FMath::IsNearlyEqual(Angle, V.Angle, Tolerance);
		}
	};
    
    template <typename T>
    requires (std::is_same_v<VectorRegister4Double, T> || std::is_same_v<VectorRegister4Float, T>)
    [[nodiscard]] constexpr T RemapRotationForCounterClockwiseRotation(const T& Rotation)
    {
        static constexpr auto RemapThreshold{
            MakeVectorRegisterConstant(180.0f - CounterClockwiseRotationAngleThreshold, 180.0f - CounterClockwiseRotationAngleThreshold,
                                        180.0f - CounterClockwiseRotationAngleThreshold, 180.0f - CounterClockwiseRotationAngleThreshold)
        };

        static constexpr auto RemapAngles{MakeVectorRegisterConstant(360.0f, 360.0f, 360.0f, 0.0f)};

        const auto ReverseRotationMask{VectorCompareGE(Rotation, RemapThreshold)};

        const auto ReversedRotation{VectorSubtract(Rotation, RemapAngles)};

        return VectorSelect(ReverseRotationMask, ReversedRotation, Rotation);
    }
    
	template <typename T>
	[[nodiscard]] constexpr T Lerp(const T& Current, const T& Target, float Ratio)
	{
		if constexpr (is_instance_v<T, UE::Math::TQuat>)
		{
			return T::FastLerp(Current, Target, Ratio).GetNormalized();
		}
		else if constexpr (is_instance_v<T, UE::Math::TRotator>)
		{
#if PLATFORM_ENABLE_VECTORINTRINSICS
		    const auto CurrentRegister{VectorLoadFloat3_W0(&Current)};
		    const auto TargetRegister{VectorLoadFloat3_W0(&Target)};

		    auto Delta{VectorSubtract(TargetRegister, CurrentRegister)};
		    Delta = VectorNormalizeRotator(Delta);

		    Delta = RemapRotationForCounterClockwiseRotation(Delta);

		    auto ResultRegister{VectorMultiplyAdd(Delta, VectorLoadFloat1(&Ratio), CurrentRegister)};
		    ResultRegister = VectorNormalizeRotator(ResultRegister);

		    T Result;
		    VectorStoreFloat3(ResultRegister, &Result);

		    return Result;
#else
		    auto Result{Target - Current};
		    Result.Normalize();

		    Result.Pitch = RemapAngleForCounterClockwiseRotation(Result.Pitch);
		    Result.Yaw = RemapAngleForCounterClockwiseRotation(Result.Yaw);
		    Result.Roll = RemapAngleForCounterClockwiseRotation(Result.Roll);

		    Result *= Ratio;
		    Result += Current;
		    Result.Normalize();

		    return Result;
#endif
		}
		else if constexpr (is_instance_v<T, TAngle>)
		{
			auto Delta{FMath::UnwindDegrees(*Target - *Current)};
			Delta = RemapAngleForCounterClockwiseRotation(Delta);

			return T{FMath::UnwindDegrees(*Current + Delta * Ratio)};
		}
		else
		{
			return FMath::Lerp(Current, Target, Ratio);
		}
	}

	/**
	 * @param Current
	 * @param Target
	 * @param HalfLife half of the time it takes from Current to Target / half of the time it takes to complete interpolation
	 * @param DeltaTime
	 *
	 * @see https://theorangeduck.com/page/spring-roll-call#exactdamper
	 */
	template <typename T>
	[[nodiscard]] constexpr T DamperExact(const T& Current, const T& Target, const float DeltaTime, const float HalfLife)
	{
        if (CompareValue<ERemComparisonOperator::Equals>(Current, Target))
        {
            return Target;
        }
        
		return Lerp(Current, Target, DamperExact(DeltaTime, HalfLife));
	}

namespace Private
{
        
    template <typename T>
    requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr UE::Math::TRotator<T> DampRotatorWithHalfLife3(const UE::Math::TRotator<T>& Current, const UE::Math::TRotator<T>& Target,
        const float DeltaTime, const FVector3f HalfLife3)
    {
        TAngle<T> CurrentPitch{Current.Pitch}, CurrentYaw{Current.Pitch}, CurrentRoll{Current.Roll};
        TAngle<T> TargetPitch{Target.Pitch}, TargetYaw{Target.Pitch}, TargetRoll{Target.Roll};

        TAngle<T> ResultPitch = Math::DamperExact(CurrentPitch, TargetPitch, DeltaTime, HalfLife3.X);
        TAngle<T> ResultYaw = Math::DamperExact(CurrentYaw, TargetYaw, DeltaTime, HalfLife3.Y);
        TAngle<T> ResultRoll = Math::DamperExact(CurrentRoll, TargetRoll, DeltaTime, HalfLife3.Z);

        UE::Math::TRotator<T> Result = {*ResultPitch, *ResultYaw, *ResultRoll};
        Result.Normalize();
        
        return Result;
    }

}
    
    template <typename T>
    requires (Rem::is_instance_v<T, UE::Math::TRotator>
        || Rem::is_instance_v<T, UE::Math::TQuat>
        || Rem::is_instance_v<T, UE::Math::TVector>
        )
	[[nodiscard]] constexpr T DamperExact(const T& Current, const T& Target, const float DeltaTime, const FVector3f HalfLife3)
	{
        if (CompareValue<ERemComparisonOperator::Equals>(Current, Target))
        {
            return Target;
        }

        if (HalfLife3.AllComponentsEqual())
        {
            return Lerp(Current, Target, DamperExact(DeltaTime, HalfLife3.X));
        }
        
        T Result;
        if constexpr (Rem::is_instance_v<T, UE::Math::TRotator>)
        {
            Result = Private::DampRotatorWithHalfLife3(Current, Target, DeltaTime, HalfLife3);
        }
        else if constexpr (Rem::is_instance_v<T, UE::Math::TQuat>)
        {
            auto CurrentRotator{Current.Rotator()};
            auto TargetRotator{Target.Rotator()};

            auto ResultRotator{Private::DampRotatorWithHalfLife3(CurrentRotator, TargetRotator, DeltaTime, HalfLife3)};
            
            Result = ResultRotator.Quaternion();
        }
        else if constexpr (Rem::is_instance_v<T, UE::Math::TVector>)
        {
            Result = {
                Math::DamperExact(Current.X, Target.X, DeltaTime, HalfLife3.X),
                Math::DamperExact(Current.Y, Target.Y, DeltaTime, HalfLife3.Y),
                Math::DamperExact(Current.Z, Target.Z, DeltaTime, HalfLife3.Z),
            };
        }
        else
        {
            static_assert(Rem::always_false<T>::value, "there might be a better way to handle it");
            
            Result = Lerp(Current, Target, DamperExact(DeltaTime, HalfLife3.X));
        }
        
        return Result;
	}

	template <typename ValueType, typename StateType>
	[[nodiscard]] constexpr ValueType SpringDamper(StateType& SpringState, const ValueType& Current, const ValueType& Target, const float DeltaTime,
		const float Frequency, const float DampingRatio, const float TargetVelocityAmount)
	{
		if (DeltaTime <= FiveDigitsAfterDecimalPoint)
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
	[[nodiscard]] constexpr auto CombineRotator(const UE::Math::TRotator<T>& Left, const UE::Math::TRotator<T>& Right) -> auto
	{
		return UE::Math::TRotator<T>{Right.Quaternion() * Left.Quaternion()};
	}

	template <typename T>
	requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr UE::Math::TVector2<T> RadianToDirectionXY(T Radian)
	{
		T Sin, Cos;
		FMath::SinCos(&Sin, &Cos, Radian);

		return {Cos, Sin};
	}

	template <typename T>
	requires std::is_floating_point_v<T>
	[[nodiscard]] constexpr UE::Math::TVector2<T> AngleToDirectionXY(T Angle)
	{
		return RadianToDirectionXY(FMath::DegreesToRadians(Angle));
	}

	namespace Private
	{
		template <typename T>
		requires std::is_floating_point_v<T>
		constexpr void PerpendicularClockwiseXY(T& X, T& Y)
		{
			std::swap(X, Y);
			Y *= -1.0f;
		}

		template <typename T>
		requires std::is_floating_point_v<T>
		constexpr void PerpendicularCounterClockwiseXY(T& X, T& Y)
		{
			std::swap(X, Y);
			X *= -1.0f;
		}
	}

	template <typename T>
	requires (
		is_instance_v<T, UE::Math::TVector>
		|| is_instance_v<T, UE::Math::TVector2>
		|| is_instance_v<T, UE::Math::TVector4>
		)
	[[nodiscard]] constexpr T PerpendicularClockwiseXY(const T& Vector)
	{
		auto Result{Vector};
		Private::PerpendicularClockwiseXY(Result.X, Result.Y);
		return Result;
	}

	template <typename T>
	requires (
		is_instance_v<T, UE::Math::TVector>
		|| is_instance_v<T, UE::Math::TVector2>
		|| is_instance_v<T, UE::Math::TVector4>
		)
	[[nodiscard]] constexpr T PerpendicularCounterClockwiseXY(const T& Vector)
	{
		auto Result{Vector};
		Private::PerpendicularCounterClockwiseXY(Result.X, Result.Y);
		return Result;
	}

    template <typename T>
	requires (std::is_floating_point_v<T> || std::is_integral_v<T>)
    [[nodiscard]] constexpr T GetWrappedValueInRange(const T Value, UE::Math::TVector2<T> ValueRange)
    {
        const auto Range = ValueRange.Y - ValueRange.X;
    
        return ValueRange.X + FMath::Modulo(Value, Range);
    }
}
