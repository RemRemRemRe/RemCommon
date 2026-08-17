// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "RemReflectedFunctionTestStructs.generated.h"

namespace Rem::Common::Private
{
/**
 * Test target for reflected function execution.
 */
UCLASS()
class URemReflectedFunctionTestTarget : public UObject
{
    GENERATED_BODY()

public:
    int32 LastValue{};

    UFUNCTION()
    void SetLastValue(int32 Value)
    {
        LastValue = Value;
    }

    UFUNCTION()
    static int32 GetStaticAnswer()
    {
        return 42;
    }
};
}
