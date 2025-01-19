// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Interface/RemInterfaceStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemInterfaceStatics)

bool URemInterfaceStatics::IsValidInterface(const FScriptInterface Interface)
{
	return IsValid(Interface.GetObject());
}

