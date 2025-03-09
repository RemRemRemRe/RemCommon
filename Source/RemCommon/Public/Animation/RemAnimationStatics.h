// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RemAnimationStatics.generated.h"

class UAnimInstance;

UCLASS()
class REMCOMMON_API URemAnimationStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
};

namespace Rem::Animation
{
	/**
	 * try get anim instance from ACharacter::GetMesh
	 */
	REMCOMMON_API UAnimInstance* GetAnimInstance(const AActor& Actor);
}
