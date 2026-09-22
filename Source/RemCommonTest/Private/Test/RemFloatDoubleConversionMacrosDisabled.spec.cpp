// Copyright RemRemRemRe. 2026. All Rights Reserved.

// Pins the disabled mode of the macro family: with the conversion enabled off,
// every macro forwards its arguments unchanged, keeping their width. This is the
// only TU that exercises that branch — the header defaults to on.
#define REM_ENABLE_FLOAT_DOUBLE_CONVERSION_MACRO 0

#include "Macro/RemFloatDoubleConversionMacros.h"

#include <type_traits>
#include <utility>

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemFloatDoubleConversionMacrosDisabledTest, "Rem.Common.Macros.FloatDoubleConversionDisabled",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemFloatDoubleConversionMacrosDisabledTest::Define()
{
    Describe(TEXT("Float/double conversion macros (conversion enabled off)"), [this]
    {
        It(TEXT("should forward an existing value without changing its width"), [this]
        {
            const FVector3f NarrowVector{1.0f, 2.0f, 3.0f};
            const FRotator3f NarrowRotator{0.0f, 90.0f, 0.0f};
            const FRotator3d WideRotator{0.0, 90.0, 0.0};

            const auto ForwardedVector      = REM_VECTOR3_DOUBLE(NarrowVector);
            const auto ForwardedRotator     = REM_ROTATOR_DOUBLE(NarrowRotator);
            const auto ForwardedWideRotator = REM_ROTATOR_FLOAT(WideRotator);

            static_assert(std::is_same_v<std::decay_t<decltype(ForwardedVector)>, FVector3f>);
            static_assert(std::is_same_v<std::decay_t<decltype(ForwardedRotator)>, FRotator3f>);
            static_assert(std::is_same_v<std::decay_t<decltype(ForwardedWideRotator)>, FRotator3d>);

            TestEqual(TEXT("forwarded vector z"), ForwardedVector.Z, 3.0f);
            TestEqual(TEXT("forwarded rotator yaw"), ForwardedRotator.Yaw, 90.0f);
            TestEqual(TEXT("forwarded wide rotator yaw"), ForwardedWideRotator.Yaw, 90.0);
        });

        It(TEXT("should forward every macro without changing the argument's own width"), [this]
        {
            static_assert(std::is_same_v<decltype(REM_VECTOR2_DOUBLE(std::declval<const FVector2f&>())), const FVector2f&>);
            static_assert(std::is_same_v<decltype(REM_VECTOR2_FLOAT(std::declval<const FVector2d&>())), const FVector2d&>);
            static_assert(std::is_same_v<decltype(REM_VECTOR3_DOUBLE(std::declval<const FVector3f&>())), const FVector3f&>);
            static_assert(std::is_same_v<decltype(REM_VECTOR3_FLOAT(std::declval<const FVector3d&>())), const FVector3d&>);
            static_assert(std::is_same_v<decltype(REM_VECTOR4_DOUBLE(std::declval<const FVector4f&>())), const FVector4f&>);
            static_assert(std::is_same_v<decltype(REM_VECTOR4_FLOAT(std::declval<const FVector4d&>())), const FVector4d&>);
            static_assert(std::is_same_v<decltype(REM_ROTATOR_DOUBLE(std::declval<const FRotator3f&>())), const FRotator3f&>);
            static_assert(std::is_same_v<decltype(REM_ROTATOR_FLOAT(std::declval<const FRotator3d&>())), const FRotator3d&>);
            static_assert(std::is_same_v<decltype(REM_QUAT_DOUBLE(std::declval<const FQuat4f&>())), const FQuat4f&>);
            static_assert(std::is_same_v<decltype(REM_QUAT_FLOAT(std::declval<const FQuat4d&>())), const FQuat4d&>);

            const FVector4f Vector4{1.0f, 2.0f, 3.0f, 4.0f};
            const auto Forwarded = REM_VECTOR4_DOUBLE(Vector4);

            TestEqual(TEXT("forwarded vector4 w"), Forwarded.W, 4.0f);
        });

        It(TEXT("should still accept an argument list inside a braced construction"), [this]
        {
            const auto Vector = FVector3d{REM_VECTOR3_DOUBLE(1.0, 2.0, 3.0)};

            static_assert(std::is_same_v<std::decay_t<decltype(Vector)>, FVector3d>);

            TestEqual(TEXT("vector z"), Vector.Z, 3.0);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
