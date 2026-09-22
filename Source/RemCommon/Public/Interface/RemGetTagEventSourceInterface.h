// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "RemGetTagEventSourceInterface.generated.h"

class IRemTagEventSourceInterface;

/**
 * getter interface: view target actors implement this to provide their tag event source
 *
 * @see IRemGetTeamInterface / IRemTeamInterface for the same getter pattern
 */
UINTERFACE(MinimalAPI)
class URemGetTagEventSourceInterface : public UInterface
{
    GENERATED_BODY()
};

class REMCOMMON_API IRemGetTagEventSourceInterface
{
    GENERATED_BODY()

public:
    [[nodiscard]] virtual const IRemTagEventSourceInterface* GetTagEventSource() const = 0;
    [[nodiscard]] virtual IRemTagEventSourceInterface* GetTagEventSource() = 0;
};
