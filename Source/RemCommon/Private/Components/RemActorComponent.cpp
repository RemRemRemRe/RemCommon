// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "Components/RemActorComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemActorComponent)

URemActorComponent::URemActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}
