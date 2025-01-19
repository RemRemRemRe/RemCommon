// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Interface/RemStringInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemStringInterface)


FString IStringInterface::ToString() const
{
	// if you don't want to print in some case like shipping,
	// could change this behavior here
	return ToStringInternal();
}

