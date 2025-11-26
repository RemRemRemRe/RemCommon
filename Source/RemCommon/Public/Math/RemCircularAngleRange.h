// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Macro/RemAssertionMacros.h"
#include "Macro/RemMacroUtilities.h"

#include "Math/Rotator.h"

struct FRemCircularAngleRange
{
 
private:
    /**
     * normalized angle : [0, 360)
     */
    float StartAngle;
    
    /**
     * normalized angle : [0, 360)
     */
    float EndAngle;
    
public:
    /**
     * default to a full circle
     */
    FRemCircularAngleRange() 
        : StartAngle(0.0f), EndAngle(0.0f)
    {
    }
    
    /**
     * @param Center could be arbitrary angle, would be converted to [0, 360)
     * @param FullSpan full range of the angle, must be greater than 0
     */
    FRemCircularAngleRange(const float Center, const float FullSpan)
    {
        SetRangeFromCenterAndSpan(Center, FullSpan);
    }

    /**
     * @param AngleRange could be arbitrary angle range
     * @param bClockwise 
     */
    explicit FRemCircularAngleRange(const FVector2f AngleRange, const bool bClockwise = true)
    {
        SetRangeFromAngles(AngleRange.X, AngleRange.Y, bClockwise);
    }
    
    /**
     * @param Center could be arbitrary angle, would be converted to [0, 360)
     * @param FullSpan full range of the angle, must be greater than 0
     */
    void SetRangeFromCenterAndSpan(float Center, const float FullSpan)
    {
#if REM_WITH_DEVELOPMENT_ONLY_CODE
        
        RemCheckCondition(FullSpan >= 0.0f, return);
        
#endif
        
        if (FullSpan >= 360.0f)
        {
            StartAngle = 0.0f;
            EndAngle = 0.0f;
            return;
        }
        
        Center = FRotator3f::ClampAxis(Center);
        const float HalfSpan = FMath::Clamp(FullSpan * 0.5f, 0.0f, 180.0f);

        const float RawStart = Center - HalfSpan;
        const float RawEnd = Center + HalfSpan;
        
        StartAngle = FRotator3f::ClampAxis(RawStart);
        EndAngle = FRotator3f::ClampAxis(RawEnd);
    }
    
    /**
     * @param AngleA could be arbitrary angle
     * @param AngleB could be arbitrary angle
     * @param bClockwise 
     */
    void SetRangeFromAngles(float AngleA, float AngleB, const bool bClockwise = true)
    {
        if (IsFullCircleByAngles(AngleA, AngleB))
        {
            StartAngle = 0.0f;
            EndAngle = 0.0f;
            return;
        }
        
        AngleA = FRotator3f::ClampAxis(AngleA);
        AngleB = FRotator3f::ClampAxis(AngleB);
        
        if (bClockwise)
        {
            StartAngle = AngleA;
            EndAngle = AngleB;
        }
        else
        {
            StartAngle = AngleB;
            EndAngle = AngleA;
        }
    }
    
    /**
     * @param Angle could be arbitrary angle
     * 
     * @return true if it's in the range
     */
    bool Contains(float Angle) const
    {
        if (IsFullCircle())
        {
            return true;
        }
        
        Angle = FRotator3f::ClampAxis(Angle);
        
        if (!IsWrappingZero())
        {
            return (Angle >= StartAngle) && (Angle <= EndAngle);
        }
        
        // check if the angle lands in [StartAngle, 360) or [0, EndAngle]
        return (Angle >= StartAngle) || (Angle <= EndAngle);
    }
    
    /**
     * @param Angle could be arbitrary angle
     * 
     * @return value in range : [0, 360)
     */
    float ClampAngle(float Angle) const
    {
        Angle = FRotator3f::ClampAxis(Angle);
        
        if (IsFullCircle() || Contains(Angle))
        {
            return Angle;
        }
        
        const float DistToStart = GetShortestAngleDistance(Angle, StartAngle);
        const float DistToEnd = GetShortestAngleDistance(Angle, EndAngle);
        return (DistToStart < DistToEnd) ? StartAngle : EndAngle;
    }
    
    float GetStartAngle() const
    {
        return StartAngle;
    }

    float GetEndAngle() const
    {
        return EndAngle;
    }
    
    /**
     * @return value in range : [0, 360)
     */
    FVector2f GetBounds() const
    {
        return {StartAngle, EndAngle};
    }
    
    /**
     * @return value in range : [0, 360)
     */
    float GetCenter() const
    {
        if (IsFullCircle())
        {
            return 0.0f;
        }
        
        return FRotator3f::ClampAxis(StartAngle + GetSpan() * 0.5f);
    }
    
    /**
     * @return value in range : [0, 360)
     */
    float GetSpan() const
    {
        if (IsFullCircle())
        {
            return 360.0f;
        }
        
        if (!IsWrappingZero())
        {
            return EndAngle - StartAngle;
        }
        
        return (360.0f - StartAngle) + EndAngle;
    }
    
    float GetRange() const
    {
        return GetSpan();
    }
    
    /**
     * @return true if the range contains the 0°
     */
    bool IsWrappingZero() const
    {
        if (IsFullCircle())
        {
            return true;
        }
        
        return StartAngle > EndAngle;
    }
    
    /**
     * @return turn if the range contains full 360°
     */
    bool IsFullCircle() const
    {
        return Rem::Math::IsNearlyZero(StartAngle) && Rem::Math::IsNearlyZero(EndAngle);
    }
    
    FString ToString() const
    {
        if (IsFullCircle())
        {
            return FString("[0°-360°]");
        }
        
        if (!IsWrappingZero())
        {
            return FString::Printf(TEXT("[%.3f°-%.3f°]"), StartAngle, EndAngle);
        }
        
        return FString::Printf(TEXT("[%.3f°-360°) and [0°-%.3f°]"), StartAngle, EndAngle);
    }
    
    /**
     * @return angle distance of shortest path for two angles,
     * always greater than 0
     * 
     * @note angle is supposed to be ClampAxis -ed
     */
    static float GetShortestAngleDistance(const float From, const float To)
    {
        const float Diff = FMath::Abs(From - To);
        return FMath::Min(Diff, 360.0f - Diff);
    }

    /**
     * @return shortest angle representation for the input angle, NOT ClampAxis -ed,
     *  eg: input: 360.0f, output: 0.0f; input: 270.0f, output: -90.0f
     */
    static float GetShortestAngle(const float Angle)
    {
        const auto AngleOfOtherDirection = FMath::Modulo(Angle - 360.0f, 360.0f);

        const auto Abs = FMath::Abs(Angle);
        const auto AbsOfOtherDirection = FMath::Abs(AngleOfOtherDirection);
        
        return Abs < AbsOfOtherDirection ? Angle : AngleOfOtherDirection;
    }

    /**
     * angle is NOT supposed to be ClampAxis -ed
     */
    static bool IsFullCircleByAngles(const float A, const float B)
    {
        const auto ClampedA = FRotator3f::ClampAxis(A);
        const auto ClampedB = FRotator3f::ClampAxis(B);
        
        if (!Rem::Math::IsNearlyEqual(ClampedA, ClampedB))
        {
            return false;
        }
        
        const auto RawDiff = FMath::Abs(A - B);
        
        return Rem::Math::IsNearlyEqual(FMath::Fmod(RawDiff, 360.0f), 0.0f) &&
           !Rem::Math::IsNearlyZero(RawDiff);
    }
};
