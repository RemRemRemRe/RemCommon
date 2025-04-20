// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemStructAsComponent.h"
#include "Components/RemStructAsComponent.inl"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemStructAsComponent)

void FRemStructAsComponentBase::Initialize(UObject& Owner)
{
}

bool FRemStructAsComponentBase::ShouldTick(UObject& Owner) const
{
	return false;
}

void FRemStructAsComponentBase::Tick(UObject& Owner, float)
{
}

void FRemStructAsComponentBase::Uninitialize(UObject& Owner)
{
}

void FRemStructAsComponents::Initialize(UObject& Owner)
{
	ForEachComponent<FRemStructAsComponentBase>(
		[&] (FRemStructAsComponentBase& Component)
	 {
		 Component.Initialize(Owner);
	 });
}

void FRemStructAsComponents::Uninitialize(UObject& Owner)
{
	ForEachComponent<FRemStructAsComponentBase>(
		[&] (FRemStructAsComponentBase& Component)
	 {
		 Component.Uninitialize(Owner);
	 });
}
