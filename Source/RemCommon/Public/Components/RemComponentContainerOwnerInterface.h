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

/**
 * @note the default implementation for GetComponentContainerView is not sufficient,
 * if you returned derived type of FRemComponentContainer, GIVE YOUR OWN implementation in the case
 */
class REMCOMMON_API IRemComponentContainerOwnerInterface
{
	GENERATED_BODY()

public:
	virtual TConstStructView<FRemComponentContainer> GetComponentContainerView() const;
	virtual TStructView<FRemComponentContainer> GetComponentContainerView();

	virtual FRemComponentContainer& GetComponentContainer() = 0;
	virtual const FRemComponentContainer& GetComponentContainer() const = 0;
};
