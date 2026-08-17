// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Enum/RemAngleCombination.h"
#include "Enum/RemArrayOperation.h"
#include "Enum/RemAxisCombination.h"
#include "Enum/RemComparisonOperator.h"
#include "Enum/RemContainerCombination.h"

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

using namespace Rem::Enum::BitOperation;

DEFINE_SPEC(FRemEnumCombinationsTest, "Rem.Common.EnumCombinations",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemEnumCombinationsTest::Define()
{
    Describe(TEXT("ERemAngleCombination"), [this]
    {
        It(TEXT("should combine through the bit operation traits"), [this]
        {
            const auto PitchYaw = ERemAngleCombination::Pitch | ERemAngleCombination::Yaw;

            TestEqual(TEXT("pitch yaw"), PitchYaw, ERemAngleCombination::PitchYaw);
            TestTrue(TEXT("has pitch"),
                static_cast<bool>(PitchYaw & ERemAngleCombination::Pitch));
            TestTrue(TEXT("has yaw"),
                static_cast<bool>(PitchYaw & ERemAngleCombination::Yaw));
            TestFalse(TEXT("no roll"),
                static_cast<bool>(PitchYaw & ERemAngleCombination::Roll));
            TestEqual(TEXT("all"), ERemAngleCombination::All, ERemAngleCombination::Pitch |
                ERemAngleCombination::Yaw | ERemAngleCombination::Roll);
        });
    });

    Describe(TEXT("ERemAxisCombination"), [this]
    {
        It(TEXT("should combine through the bit operation traits"), [this]
        {
            const auto XZ = ERemAxisCombination::X | ERemAxisCombination::Z;

            TestEqual(TEXT("x z"), XZ, ERemAxisCombination::XZ);
            TestTrue(TEXT("has x"), static_cast<bool>(XZ & ERemAxisCombination::X));
            TestTrue(TEXT("has z"), static_cast<bool>(XZ & ERemAxisCombination::Z));
            TestFalse(TEXT("no y"), static_cast<bool>(XZ & ERemAxisCombination::Y));
        });
    });

    Describe(TEXT("ERemContainerCombination"), [this]
    {
        It(TEXT("should combine container flags through the bit operation traits"), [this]
        {
            const auto ArrayAndSet = Rem::Enum::EContainerCombination::Array
                | Rem::Enum::EContainerCombination::Set;

            TestTrue(TEXT("has array"),
                static_cast<bool>(ArrayAndSet & Rem::Enum::EContainerCombination::Array));
            TestTrue(TEXT("has set"),
                static_cast<bool>(ArrayAndSet & Rem::Enum::EContainerCombination::Set));
        });
    });

    Describe(TEXT("ERemArrayOperation"), [this]
    {
        It(TEXT("should insert data at the front or the back of the reference element"), [this]
        {
            TArray<int32> Numbers{1, 3};

            // PushBack relative to the reference element 1: insert after it
            Rem::Object::ApplyArrayOperation<ERemArrayOperation::PushBack>(Numbers, 1, {2});
            TestEqual(TEXT("pushed after reference"), Numbers[1], 2);

            // InsertFront relative to the reference element 3: insert before it
            Rem::Object::ApplyArrayOperation<ERemArrayOperation::InsertFront>(Numbers, 3, {0});
            TestEqual(TEXT("inserted before reference"), Numbers[2], 0);
        });
    });

    Describe(TEXT("ERemComparisonOperator"), [this]
    {
        It(TEXT("CompareValue should evaluate every operator"), [this]
        {
            using Rem::Math::CompareValue;

            TestTrue(TEXT("equals"), CompareValue<ERemComparisonOperator::Equals>(1, 1));
            TestFalse(TEXT("equals false"), CompareValue<ERemComparisonOperator::Equals>(1, 2));
            TestTrue(TEXT("not equal"), CompareValue<ERemComparisonOperator::NotEqual>(1, 2));
            TestTrue(TEXT("less than"), CompareValue<ERemComparisonOperator::LessThan>(1, 2));
            TestTrue(TEXT("less or equal"), CompareValue<ERemComparisonOperator::LessThanOrEqual>(2, 2));
            TestTrue(TEXT("greater than"), CompareValue<ERemComparisonOperator::GreaterThan>(2, 1));
            TestTrue(TEXT("greater or equal"), CompareValue<ERemComparisonOperator::GreaterThanOrEqual>(2, 2));
        });

        It(TEXT("CompareValue should respect the error tolerance"), [this]
        {
            using Rem::Math::CompareValue;

            TestTrue(TEXT("within tolerance"),
                CompareValue<ERemComparisonOperator::Equals>(1.0f, 1.00001f, 1e-3f));
            TestFalse(TEXT("outside tolerance"),
                CompareValue<ERemComparisonOperator::Equals>(1.0f, 1.1f, 1e-3f));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
