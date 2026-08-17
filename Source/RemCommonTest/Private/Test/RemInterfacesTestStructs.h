// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interface/RemScriptStructInterface.h"
#include "Interface/RemStringInterface.h"

#include "RemInterfacesTestStructs.generated.h"

namespace Rem::Common::Private
{
/**
 * Test implementer of IRemScriptStructInterface: returns the empty struct.
 */
UCLASS()
class URemTestScriptStructObject : public UObject, public IRemScriptStructInterface
{
    GENERATED_BODY()

public:
    virtual Rem::TNotNull<const UScriptStruct*> GetScriptStruct() const override;
};

/**
 * Test implementer of IRemStringInterface: returns a fixed string.
 */
UCLASS()
class URemTestStringObject : public UObject, public IRemStringInterface
{
    GENERATED_BODY()

public:
    virtual FString ToString() const override;
};
}
