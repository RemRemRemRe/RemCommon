// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemCommonFunctor.h"

#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemCommonFunctorTest, "Rem.Common.CommonFunctor",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemCommonFunctorTest::Define()
{
    Describe(TEXT("RefToPtr"), [this]
    {
        It(TEXT("should convert a reference wrapper to a pointer"), [this]
        {
            auto Value = 42;
            auto Reference = std::ref(Value);

            auto* Pointer = Rem::RefToPtr(Reference);

            TestEqual(TEXT("pointer"), *Pointer, 42);
            TestTrue(TEXT("same object"), Pointer == &Value);
        });

        It(TEXT("functor version should be usable as a callable"), [this]
        {
            auto Value = 7;
            auto Reference = std::ref(Value);

            // Rem::Fn::RefToPtr is a constexpr functor object wrapping RefToPtr
            const auto Pointer = Rem::Fn::RefToPtr(Reference);

            TestTrue(TEXT("same pointer"), Pointer == &Value);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
