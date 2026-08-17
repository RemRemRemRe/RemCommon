// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Object/RemWorldCollisionAsync.h"

#include "Engine/HitResult.h"
#include "Engine/OverlapResult.h"
#include "Misc/AutomationTest.h"
#include "WorldCollision.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemWorldCollisionAsyncTest, "Rem.Common.WorldCollisionAsync",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemWorldCollisionAsyncTest::Define()
{
    Describe(TEXT("GetResultView"), [this]
    {
        It(TEXT("should return an empty view for an empty trace datum"), [this]
        {
            const FTraceDatum Datum{};

            TestTrue(TEXT("trace view empty"), Rem::Collision::GetResultView(Datum).IsEmpty());
        });

        It(TEXT("should return an empty view for an empty overlap datum"), [this]
        {
            const FOverlapDatum Datum{};

            TestTrue(TEXT("overlap view empty"), Rem::Collision::GetResultView(Datum).IsEmpty());
        });
    });

    Describe(TEXT("GetTraceActor"), [this]
    {
        It(TEXT("should return null for an empty overlap result"), [this]
        {
            const FOverlapResult Result;

            TestNull(TEXT("no actor"), Rem::Collision::GetTraceActor(Result));
        });

        It(TEXT("should return null for an empty hit result"), [this]
        {
            const FHitResult Hit;

            TestNull(TEXT("no actor"), Rem::Collision::GetTraceActor(Hit));
        });
    });

    Describe(TEXT("GetTracePrimitive"), [this]
    {
        It(TEXT("should return null for an empty overlap result"), [this]
        {
            const FOverlapResult Result;

            TestNull(TEXT("no primitive"), Rem::Collision::GetTracePrimitive(Result));
        });
    });

    // note: QueryTraceData* / QueryOverlapData* require a live world and a
    // valid FTraceHandle from an async trace request - covered by integration
    // tests when a full physics world fixture exists.
}

#endif // WITH_DEV_AUTOMATION_TESTS
