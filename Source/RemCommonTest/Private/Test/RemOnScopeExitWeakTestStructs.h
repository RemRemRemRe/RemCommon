// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "RemOnScopeExitWeak.h"
#include "UObject/Object.h"

#include "RemOnScopeExitWeakTestStructs.generated.h"

namespace Rem::Common::Private
{
/**
 * Test object running the weak scope-exit macro inside a member function.
 */
UCLASS()
class URemOnScopeExitWeakTestObject : public UObject
{
    GENERATED_BODY()

public:
    bool bExecuted{};

    void Run()
    {
        REM_ON_SCOPE_EXIT_WEAK
        {
            bExecuted = true;
        };
    }
};
}
