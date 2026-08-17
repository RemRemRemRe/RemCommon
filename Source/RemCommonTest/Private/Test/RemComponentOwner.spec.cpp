// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Components/RemActorComponent.h"
#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainerOwnerInterface.h"

#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "RemTestWorld.h"

#include "Test/RemComponentOwnerTestStructs.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::Common::Private::URemComponentOwnerTestObject;

DEFINE_SPEC(FRemComponentOwnerTest, "Rem.Common.ComponentOwner",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemComponentOwnerTest::Define()
{
    Describe(TEXT("URemActorComponent"), [this]
    {
        It(TEXT("should construct on an actor"), [this]
        {
            Rem::Test::FRemTestWorld World;

            auto* Actor = World->SpawnActor<AActor>();
            check(Actor);

            auto* Component = NewObject<URemActorComponent>(Actor);
            TestNotNull(TEXT("component"), Component);
        });
    });

    Describe(TEXT("IRemComponentContainerOwnerInterface"), [this]
    {
        It(TEXT("should return the owner container through the interface"), [this]
        {
            URemComponentOwnerTestObject* Owner = NewObject<URemComponentOwnerTestObject>();

            IRemComponentContainerOwnerInterface* Interface = Owner;
            TestTrue(TEXT("same container"),
                static_cast<FRemComponentContainer*>(Interface->GetComponentContainer()) == &Owner->Container);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
