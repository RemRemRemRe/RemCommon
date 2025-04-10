// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemActorComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemActorComponent)

URemActorComponent::URemActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	bAutoActivate = false;
}
