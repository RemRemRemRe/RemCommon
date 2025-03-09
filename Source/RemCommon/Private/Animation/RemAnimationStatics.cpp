// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Animation/RemAnimationStatics.h"

#include "RemCommonLog.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Macro/RemLogMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemAnimationStatics)

namespace Rem::Animation
{

UAnimInstance* GetAnimInstance(const AActor& Actor)
{
	if (auto* Character = Cast<ACharacter>(&Actor);
		LIKELY(Character))
	{
		if (LIKELY(Character->GetMesh()))
		{
			return Character->GetMesh()->GetAnimInstance();
		}

		REM_LOG_FUNCTION(LogRemCommon, Error, TEXT("mesh is missing on Character:{0}"), Character);
	}
	else
	{
		REM_LOG_FUNCTION(LogRemCommon, Error, TEXT("Actor:{0} is not an ACharacter "), Actor);
	}

	return nullptr;
}

}
