// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainer.inl"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentContainer)

void FRemComponentBase::Initialize(UObject& Owner)
{
}

bool FRemComponentBase::ShouldTick(UObject& Owner) const
{
	return false;
}

void FRemComponentBase::Tick(UObject& Owner, float)
{
}

void FRemComponentBase::Uninitialize(UObject& Owner)
{
}

void FRemComponentContainer::Initialize(UObject& Owner)
{
	ForEachComponent<FRemComponentBase>(
		[&] (FRemComponentBase& Component)
	 {
		 Component.Initialize(Owner);
	 });
}

void FRemComponentContainer::Uninitialize(UObject& Owner)
{
	ForEachComponent<FRemComponentBase>(
		[&] (FRemComponentBase& Component)
	 {
		 Component.Uninitialize(Owner);
	 });
}
