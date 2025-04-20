// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"

#include "RemComponentContainerOwnerInterface.generated.h"

struct FRemComponentContainer;
template<typename BaseStructT>
struct TConstStructView;
template<typename BaseStructT>
struct TStructView;

UINTERFACE(MinimalAPI)
class URemComponentContainerOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class REMCOMMON_API IRemComponentContainerOwnerInterface
{
	GENERATED_BODY()

public:
	virtual TConstStructView<FRemComponentContainer> GetComponentContainerView() const = 0;
	virtual TStructView<FRemComponentContainer> GetComponentContainerView() = 0;

	virtual FRemComponentContainer& GetComponentContainer() = 0;
	virtual const FRemComponentContainer& GetComponentContainer() const = 0;
};
