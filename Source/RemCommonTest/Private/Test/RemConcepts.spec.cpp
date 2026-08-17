// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemConcepts.h"
#include "RemCppConcepts.h"
#include "RemStructConcepts.h"
#include "Subsystems/RemSubsystemConcepts.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Misc/AutomationTest.h"
#include "Test/RemScopedStructContainerTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

// --- positive concept checks ----------------------------------------------

static_assert(Rem::CHasGetWorld<AActor>);
static_assert(Rem::CHasGetLocalRole<AActor>);
static_assert(Rem::CHasGetNetMode<AActor>);
static_assert(Rem::CHasGetOwner<AActor>);

// --- negative concept checks ----------------------------------------------

static_assert(!Rem::CHasGetWorld<int32>);
static_assert(!Rem::CHasToString<int32>);
static_assert(!Rem::CHasGet<FString>); // FString has no Get()

DEFINE_SPEC(FRemConceptsTest, "Rem.Common.Concepts",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemConceptsTest::Define()
{
    Describe(TEXT("Concept contracts"), [this]
    {
        It(TEXT("should hold at compile time"), [this]
        {
            // the contracts are enforced by the static_asserts above; this
            // runtime check documents the spec body
            TestTrue(TEXT("compile-time contracts verified"), true);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
