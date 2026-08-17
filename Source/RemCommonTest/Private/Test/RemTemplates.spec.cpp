// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemScopedStructContainerTestStructs.h"

#include "Enum/RemEnumStatics.inl"
#include "Enum/RemHelperEnum.h"
#include "GameplayTagsManager.h"
#include "Misc/AutomationTest.h"
#include "RemCoreStatics.inl"
#include "RemStd.inl"
#include "Struct/RemStructViewStatics.inl"
#include "Templates/RemStringBuilderHelper.inl"

#if WITH_DEV_AUTOMATION_TESTS

namespace
{
using Rem::Struct::Private::FRemScopedStructContainerTestAligned;
using Rem::Struct::Private::FRemScopedStructContainerTestTrivial;
FGameplayTag AddTag(const FStringView TagName)
{
    auto& TagsManager = UGameplayTagsManager::Get();
    if (const auto ExistingTag = TagsManager.RequestGameplayTag(FName{TagName}, false);
        ExistingTag.IsValid())
    {
        return ExistingTag;
    }
    return TagsManager.AddNativeGameplayTag(FName{TagName});
}
}

DEFINE_SPEC(FRemTemplatesTest, "Rem.Common.Templates",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemTemplatesTest::Define()
{
    Describe(TEXT("RemStd::bind_back"), [this]
    {
        It(TEXT("should append the bound arguments after the call arguments"), [this]
        {
            auto Add = [](const int32 A, const int32 B)
            {
                return A + B;
            };

            const auto Bound = RemStd::bind_back(Add, 40);

            TestEqual(TEXT("result"), Bound(2), 42);
        });

        It(TEXT("should support multiple bound arguments"), [this]
        {
            auto Sum3 = [](const int32 A, const int32 B, const int32 C)
            {
                return A + B + C;
            };

            const auto Bound = RemStd::bind_back(Sum3, 10, 20);

            TestEqual(TEXT("result"), Bound(12), 42);
        });
    });

    Describe(TEXT("RemoveInvalidKey"), [this]
    {
        It(TEXT("should remove entries with invalid keys and keep valid ones"), [this]
        {
            const auto TagA = AddTag(TEXT("Test.Common.Templates.Map.A"));
            TMap<FGameplayTag, int32> Map;
            Map.Add(TagA, 1);
            Map.Add(FGameplayTag{}, 2);

            Rem::RemoveInvalidKey(Map);

            TestEqual(TEXT("remaining count"), Map.Num(), 1);
            TestTrue(TEXT("valid key kept"), Map.Contains(TagA));
            TestEqual(TEXT("value"), Map[TagA], 1);
        });
    });

    Describe(TEXT("ApplyTransition"), [this]
    {
        It(TEXT("ToNext should step forward and clamp at the end"), [this]
        {
            TestEqual(TEXT("step"), Rem::Enum::ApplyTransition(0, Rem::Enum::ETransitionType::ToNext, 2, 10), 2);
            TestEqual(TEXT("clamped"), Rem::Enum::ApplyTransition(9, Rem::Enum::ETransitionType::ToNext, 2, 10), 10);
        });

        It(TEXT("ToPrevious should step backward and clamp at the start"), [this]
        {
            TestEqual(TEXT("step"), Rem::Enum::ApplyTransition(5, Rem::Enum::ETransitionType::ToPrevious, 2, 10), 3);
            TestEqual(TEXT("clamped"), Rem::Enum::ApplyTransition(1, Rem::Enum::ETransitionType::ToPrevious, 2, 10), 0);
        });

        It(TEXT("ToSpecified should jump to the step value"), [this]
        {
            TestEqual(TEXT("jump"), Rem::Enum::ApplyTransition(5, Rem::Enum::ETransitionType::ToSpecified, 7, 10), 7);
        });

        It(TEXT("negative step should move to the end or the start"), [this]
        {
            TestEqual(TEXT("to end"), Rem::Enum::ApplyTransition(3, Rem::Enum::ETransitionType::ToNext, -1, 10), 9);
            TestEqual(TEXT("to start"), Rem::Enum::ApplyTransition(3, Rem::Enum::ETransitionType::ToPrevious, -1, 10), 0);
        });

        It(TEXT("the tuple overload should unpack the transition data"), [this]
        {
            const TTuple<Rem::Enum::ETransitionType, int32> Data{Rem::Enum::ETransitionType::ToSpecified, 8};

            TestEqual(TEXT("unpacked"), Rem::Enum::ApplyTransition(0, Data, 10), 8);
        });

        It(TEXT("the transition constants should describe the common moves"), [this]
        {
            TestEqual(TEXT("abort"), Rem::Enum::ApplyTransition(5, Rem::Enum::Transition::Abort<int32>, 10), -1);
            TestEqual(TEXT("next"), Rem::Enum::ApplyTransition(0, Rem::Enum::Transition::ToNext<int32>, 10), 1);
            TestEqual(TEXT("previous"), Rem::Enum::ApplyTransition(5, Rem::Enum::Transition::ToPrevious<int32>, 10), 4);
            TestEqual(TEXT("to start"), Rem::Enum::ApplyTransition(5, Rem::Enum::Transition::ToStart<int32>, 10), 0);
            TestEqual(TEXT("to end"), Rem::Enum::ApplyTransition(0, Rem::Enum::Transition::ToEnd<int32>, 10), 9);
            TestEqual(TEXT("restart stays with zero step"), Rem::Enum::ApplyTransition(5, Rem::Enum::Transition::Restart<int32>, 10), 5);
        });
    });

    Describe(TEXT("Struct view helpers"), [this]
    {
        It(TEXT("MakeView should convert a base view to a typed view"), [this]
        {
            FRemScopedStructContainerTestTrivial Data;
            Data.Number = 1.5f;
            const FConstStructView BaseView{FRemScopedStructContainerTestTrivial::StaticStruct(),
                reinterpret_cast<const uint8*>(&Data)};

            const auto TypedView = Rem::Struct::MakeView<FRemScopedStructContainerTestTrivial>(BaseView);

            TestTrue(TEXT("valid"), TypedView.IsValid());
            TestEqual(TEXT("number"), TypedView.Get<const FRemScopedStructContainerTestTrivial>().Number, 1.5f,
                1e-4f);
        });

        It(TEXT("TryMakeView should reject a type mismatch"), [this]
        {
            const FConstStructView BaseView{FRemScopedStructContainerTestTrivial::StaticStruct(), nullptr};

            const auto TypedView = Rem::Struct::TryMakeView<FRemScopedStructContainerTestTrivial>(BaseView);
            TestTrue(TEXT("invalid on null memory"), !TypedView.IsValid());

            FRemScopedStructContainerTestAligned Unrelated;
            const FConstStructView WrongView{FRemScopedStructContainerTestAligned::StaticStruct(),
                reinterpret_cast<const uint8*>(&Unrelated)};
            const auto WrongTyped = Rem::Struct::TryMakeView<FRemScopedStructContainerTestTrivial>(WrongView);
            TestTrue(TEXT("invalid on wrong type"), !WrongTyped.IsValid());
        });
    });

    Describe(TEXT("AppendValueString"), [this]
    {
        It(TEXT("should format vectors, rotators and quaternions"), [this]
        {
            TAnsiStringBuilder<128> VectorBuilder;
            Rem::AppendValueString(VectorBuilder, FVector{1.0, 2.0, 3.0});
            TestEqual(TEXT("vector"), FString{VectorBuilder.ToString()}, FString{TEXT("X:1.000 Y:2.000 Z:3.000")});

            TAnsiStringBuilder<128> RotatorBuilder;
            Rem::AppendValueString(RotatorBuilder, FRotator{10.0, 20.0, 30.0});
            TestEqual(TEXT("rotator"), FString{RotatorBuilder.ToString()}, FString{TEXT("P:10.000 Y:20.000 R:30.000")});

            TAnsiStringBuilder<128> QuatBuilder;
            Rem::AppendValueString(QuatBuilder, FQuat{0.0, 0.0, 0.0, 1.0});
            TestEqual(TEXT("quat"), FString{QuatBuilder.ToString()}, FString{TEXT("X:0.000 Y:0.000 Z:0.000 W:1.000")});
        });

        It(TEXT("should format vector2 and transform"), [this]
        {
            TAnsiStringBuilder<128> Vector2Builder;
            Rem::AppendValueString(Vector2Builder, FVector2D{1.0, 2.0});
            TestEqual(TEXT("vector2"), FString{Vector2Builder.ToString()}, FString{TEXT("X:1.000 Y:2.000")});

            TAnsiStringBuilder<128> TransformBuilder;
            Rem::AppendValueString(TransformBuilder, FTransform{FQuat::Identity, FVector{1.0, 2.0, 3.0}});
            TestEqual(TEXT("transform"), FString{TransformBuilder.ToString()},
                FString{TEXT("X:1.000 Y:2.000 Z:3.000 # P:0.00 Y:0.00 R:-0.00")});
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
