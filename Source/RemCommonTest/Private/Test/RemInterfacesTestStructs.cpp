// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemInterfacesTestStructs.h"

#include "Object/RemEmptyStruct.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemInterfacesTestStructs)

namespace Rem::Common::Private
{
Rem::TNotNull<const UScriptStruct*> URemTestScriptStructObject::GetScriptStruct() const
{
    return FRemEmptyStruct::StaticStruct();
}

FString URemTestStringObject::ToString() const
{
    return FString{TEXT("RemTestStringObject")};
}
}
