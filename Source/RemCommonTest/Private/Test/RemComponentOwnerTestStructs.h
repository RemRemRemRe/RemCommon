// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainerOwnerInterface.h"

#include "RemComponentOwnerTestStructs.generated.h"

namespace Rem::Common::Private
{
/**
 * Test implementer of IRemComponentContainerOwnerInterface.
 */
UCLASS()
class URemComponentOwnerTestObject : public UObject, public IRemComponentContainerOwnerInterface
{
    GENERATED_BODY()

public:
    UPROPERTY()
    FRemComponentContainer Container;

    virtual Rem::TNotNull<FRemComponentContainer*> GetComponentContainer() override
    {
        return MakeNotNull(&Container);
    }

    virtual Rem::TNotNull<const FRemComponentContainer*> GetComponentContainer() const override
    {
        return MakeNotNull(&Container);
    }
};
}
