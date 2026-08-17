// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Object/RemNetFilterBits.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/EngineTypes.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemNetFilterBitsTest, "Rem.Common.NetFilterBits",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemNetFilterBitsTest::Define()
{
    Describe(TEXT("Matches"), [this]
    {
        It(TEXT("should allow everything by default"), [this]
        {
            constexpr FRemNetFilterBits Bits{};

            TestTrue(TEXT("client local player"),
                Bits.Matches(NM_Client, ROLE_AutonomousProxy));
            TestTrue(TEXT("client simulated proxy"),
                Bits.Matches(NM_Client, ROLE_SimulatedProxy));
            TestTrue(TEXT("standalone"), Bits.Matches(NM_Standalone, ROLE_Authority));
            TestTrue(TEXT("dedicated server"), Bits.Matches(NM_DedicatedServer, ROLE_Authority));
            TestTrue(TEXT("listen server"), Bits.Matches(NM_ListenServer, ROLE_Authority));
        });

        It(TEXT("should deny the local player when disabled"), [this]
        {
            FRemNetFilterBits Bits;
            Bits.bRunOnLocalPlayer = false;

            TestFalse(TEXT("client local player denied"),
                Bits.Matches(NM_Client, ROLE_AutonomousProxy));
            TestTrue(TEXT("simulated proxy unaffected"),
                Bits.Matches(NM_Client, ROLE_SimulatedProxy));
        });

        It(TEXT("should deny the simulated proxy when disabled"), [this]
        {
            FRemNetFilterBits Bits;
            Bits.bRunOnSimulatedProxy = false;

            TestFalse(TEXT("client simulated proxy denied"),
                Bits.Matches(NM_Client, ROLE_SimulatedProxy));
            TestTrue(TEXT("local player unaffected"),
                Bits.Matches(NM_Client, ROLE_AutonomousProxy));
        });

        It(TEXT("should deny each net mode when its bit is disabled"), [this]
        {
            FRemNetFilterBits Bits;
            Bits.bRunOnStandalone      = false;
            Bits.bRunOnDedicatedServer = false;
            Bits.bRunOnListenServer    = false;

            TestFalse(TEXT("standalone denied"), Bits.Matches(NM_Standalone, ROLE_Authority));
            TestFalse(TEXT("dedicated server denied"), Bits.Matches(NM_DedicatedServer, ROLE_Authority));
            TestFalse(TEXT("listen server denied"), Bits.Matches(NM_ListenServer, ROLE_Authority));
            TestTrue(TEXT("client unaffected"), Bits.Matches(NM_Client, ROLE_SimulatedProxy));
        });

        It(TEXT("client role bits should not affect non-client net modes"), [this]
        {
            FRemNetFilterBits Bits;
            Bits.bRunOnLocalPlayer    = false;
            Bits.bRunOnSimulatedProxy = false;

            TestTrue(TEXT("standalone unaffected"), Bits.Matches(NM_Standalone, ROLE_Authority));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
