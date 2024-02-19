// Copyright RemRemRemRe, All Rights Reserved.


#include "RemMetaTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemMetaTags)

URemMetaTags::URemMetaTags(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Put it under game section of project setting
	CategoryName = FName(TEXT("Game"));
}

