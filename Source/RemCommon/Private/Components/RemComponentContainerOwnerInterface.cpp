// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemComponentContainerOwnerInterface.h"

#include "StructUtils/StructView.h"
#include "Components/RemComponentContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentContainerOwnerInterface)

TConstStructView<FRemComponentContainer> IRemComponentContainerOwnerInterface::GetComponentContainerView() const
{
	return GetComponentContainer();
}

TStructView<FRemComponentContainer> IRemComponentContainerOwnerInterface::GetComponentContainerView()
{
	return GetComponentContainer();
}
