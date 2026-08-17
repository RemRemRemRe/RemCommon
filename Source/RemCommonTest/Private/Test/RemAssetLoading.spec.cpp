// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Object/RemAssetLoading.inl"

#include "Misc/AutomationTest.h"
#include "UObject/SoftObjectPath.h"

#if WITH_DEV_AUTOMATION_TESTS

DEFINE_SPEC(FRemAssetLoadingTest, "Rem.Common.AssetLoading",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemAssetLoadingTest::Define()
{
    Describe(TEXT("IsLoaded"), [this]
    {
        It(TEXT("should report an unloaded soft object as not loaded"), [this]
        {
            const TSoftObjectPtr<UTexture2D> SoftTexture{
                FSoftObjectPath{TEXT("/Engine/Rem/DoesNotExist.DoesNotExist")}};

            TestFalse(TEXT("not loaded"), Rem::IsLoaded(SoftTexture));
        });

        It(TEXT("should report an unresolved soft path as not loaded"), [this]
        {
            const FSoftObjectPath Path{TEXT("/Engine/Rem/DoesNotExist.DoesNotExist")};

            TestFalse(TEXT("not loaded"), Rem::IsLoaded(Path));
        });
    });

    Describe(TEXT("LoadAsset"), [this]
    {
        It(TEXT("should start an async load for a valid path and release cleanly"), [this]
        {
            const TSoftObjectPtr<UTexture2D> SoftTexture{
                FSoftObjectPath{TEXT("/Engine/Rem/DoesNotExist.DoesNotExist")}};

            const auto Handle = Rem::LoadAsset(SoftTexture);

            TestTrue(TEXT("handle valid"), Handle.IsValid());

            // release the async load state before the test ends
            Handle->CancelHandle();
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
