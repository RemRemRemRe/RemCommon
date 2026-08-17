// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Enum/RemCardinalDirection.h"
#include "Enum/RemHelperEnum.h"
#include "Enum/RemHelperEnumAlias.h"
#include "Enum/RemRangeFormat.h"
#include "Enum/RemTransitionType.h"
#include "Math/RemMathCore.h"
#include "RemAlwaysFalse.h"
#include "RemCastFn.h"
#include "RemTestWorld.h"
#include "Templates/RemEnumClassBitOperation.h"
#include "Templates/RemInstanceOf.h"
#include "Templates/RemTypeString.h"

#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

// --- compile-time checks -------------------------------------------------

static_assert(Rem::always_false<int>::value == false);
static_assert(Rem::CInstanceOf<TArray<int32>, TArray>);
static_assert(!Rem::CInstanceOf<FString, TArray>);
static_assert(std::is_same_v<decltype(Rem::Math::GetPI<float>()), float>);

namespace Rem::Enum::BitOperation
{
template <>
struct TEnumClassBitOperationTraits<ERemRangeFormat> : std::true_type
{
};
}

DEFINE_SPEC(FRemEnumUtilitiesTest, "Rem.Common.EnumUtilities",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemEnumUtilitiesTest::Define()
{
    Describe(TEXT("Cardinal directions"), [this]
    {
        It(TEXT("should pair each direction with its opposite"), [this]
        {
            using Rem::Enum::GetOppositeCardinalDirection;

            TestEqual(TEXT("forward opposite"),
                GetOppositeCardinalDirection(ERemCardinalDirection::Forward),
                ERemCardinalDirection::Backward);
            TestEqual(TEXT("backward opposite"),
                GetOppositeCardinalDirection(ERemCardinalDirection::Backward),
                ERemCardinalDirection::Forward);
            TestEqual(TEXT("left opposite"),
                GetOppositeCardinalDirection(ERemCardinalDirection::Left),
                ERemCardinalDirection::Right);
            TestEqual(TEXT("up opposite"),
                GetOppositeCardinalDirection(ERemCardinalDirection::Up),
                ERemCardinalDirection::Down);
        });

        It(TEXT("should detect perpendicular direction pairs"), [this]
        {
            using Rem::Enum::IsDirectionPerpendicular;

            TestTrue(TEXT("forward vs left"),
                IsDirectionPerpendicular(ERemCardinalDirection::Forward, ERemCardinalDirection::Left));
            TestTrue(TEXT("left vs up"),
                IsDirectionPerpendicular(ERemCardinalDirection::Left, ERemCardinalDirection::Up));
            TestFalse(TEXT("forward vs backward"),
                IsDirectionPerpendicular(ERemCardinalDirection::Forward, ERemCardinalDirection::Backward));
        });
    });

    Describe(TEXT("Transitions"), [this]
    {
        It(TEXT("should map each enum value to its transition tuple"), [this]
        {
            TestEqual(TEXT("to next type"),
                static_cast<int32>(Rem::Enum::EnumToTransition<Rem::Enum::ERemTransitionType::ToNext, int32>()
                    .template Get<Rem::Enum::ETransitionType>()),
                static_cast<int32>(Rem::Enum::ETransitionType::ToNext));
            TestEqual(TEXT("abort type"),
                static_cast<int32>(Rem::Enum::EnumToTransition<Rem::Enum::ERemTransitionType::Abort, int32>()
                    .template Get<Rem::Enum::ETransitionType>()),
                static_cast<int32>(Rem::Enum::ETransitionType::ToSpecified));
            TestEqual(TEXT("abort step"),
                Rem::Enum::EnumToTransition<Rem::Enum::ERemTransitionType::Abort, int32>().template Get<int32>(),
                -1);
        });
    });

    Describe(TEXT("Range format enum"), [this]
    {
        It(TEXT("should enumerate the three range formats"), [this]
        {
            const auto* Enum = StaticEnum<ERemRangeFormat>();

            TestEqual(TEXT("entry count"), Enum->NumEnums() - 1, 3);
            TestTrue(TEXT("start and end"),
                Enum->GetValueByNameString(TEXT("StartAndEnd")) ==
                static_cast<int64>(ERemRangeFormat::StartAndEnd));
        });
    });

    Describe(TEXT("Enum class bit operations"), [this]
    {
        It(TEXT("should combine flags through the traits"), [this]
        {
            constexpr auto StartEnd = ERemRangeFormat::StartAndEnd;
            constexpr auto LengthEnd = ERemRangeFormat::LengthAndEnd;

            // the underlying values combine as a bitmask
            const auto Combined = Rem::Enum::BitOperation::operator|(StartEnd, LengthEnd);
            const auto Intersection = Rem::Enum::BitOperation::operator&(Combined, StartEnd);

            TestEqual(TEXT("intersection"), static_cast<int32>(Intersection),
                static_cast<int32>(StartEnd));
        });
    });

    Describe(TEXT("Rem::Cast functor"), [this]
    {
        It(TEXT("should cast pointers to the target type"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            TestNotNull(TEXT("actor"), Actor);

            const auto* Casted = Rem::Cast<AActor>(static_cast<UObject*>(Actor));

            TestTrue(TEXT("casted"), Casted == Actor);
        });

        It(TEXT("should yield null for unrelated types"), [this]
        {
            Rem::Test::FRemTestWorld World;
            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            // UInputComponent is a concrete UObject but not an AActor
            auto* Component = NewObject<UInputComponent>(Actor);
            check(Component);

            TestNull(TEXT("null"), Rem::Cast<AActor>(static_cast<UObject*>(Component)));
        });
    });

    Describe(TEXT("GetTypeName"), [this]
    {
        It(TEXT("should contain the type name"), [this]
        {
            constexpr auto Name = Rem::GetTypeName<int32>();

            TestTrue(TEXT("contains int"),
                Name.find("int") != std::string_view::npos);
        });
    });

    Describe(TEXT("Helper enum aliases"), [this]
    {
        It(TEXT("should instantiate the shared alias enums"), [this]
        {
            using Rem::Enum::ECallFinishSpawn;
            using Rem::Enum::ERecursive;
            using Rem::Enum::EExcludeSelf;
            using Rem::Enum::EYesOrNo;

            ECallFinishSpawn FinishSpawn{EYesOrNo::Yes};
            ERecursive Recursive{EYesOrNo::No};
            EExcludeSelf ExcludeSelf{EYesOrNo::Yes};

            TestEqual(TEXT("finish spawn"), static_cast<uint8>(FinishSpawn.value),
                static_cast<uint8>(EYesOrNo::Yes));
            TestEqual(TEXT("recursive"), static_cast<uint8>(Recursive.value),
                static_cast<uint8>(EYesOrNo::No));
            TestEqual(TEXT("exclude self"), static_cast<uint8>(ExcludeSelf.value),
                static_cast<uint8>(EYesOrNo::Yes));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
