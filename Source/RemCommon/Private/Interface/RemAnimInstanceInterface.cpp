// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Interface/RemAnimInstanceInterface.h"

#include "Animation/RemAnimationStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemAnimInstanceInterface)

UAnimInstance* IRemAnimInstanceInterface::GetAnimInstance(const AActor& Actor) const
{
	return Rem::Animation::GetAnimInstance(Actor);
}
