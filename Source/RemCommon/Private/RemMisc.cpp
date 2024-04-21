// Copyright RemRemRemRe, All Rights Reserved.

#include "RemMisc.h"

#include "GameplayTagContainer.h"
#include "Macro/RemAssertionMacros.h"
#include "UObject/Object.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/SoftObjectPath.h"

namespace Rem
{

bool IsClassDefaultObject(const UObject* Object)
{
	RemCheckVariable(Object, {});
	
	const UClass* Class = Object->GetClass();
	return Class->GetDefaultObject(false) == Object;
}

FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath)
{
	const FString& SubPathString = SoftObjectPath.GetSubPathString();

	// may just construct a FCoreRedirectObjectName to help doing this,
	// but will that did too much extra work?
	if (int32 ObjectNameIndex;
		SubPathString.FindLastChar(TEXT('.'), ObjectNameIndex))
	{
		// + 1 to get rid of the '.'
		return SubPathString.RightChop(ObjectNameIndex + 1);
	}
	
	return {};
}

FString ToString(const UScriptStruct& ScriptStruct, const void* Value)
{
	FString HumanReadableMessage;
	ScriptStruct.ExportText(/*out*/ HumanReadableMessage, Value,
		/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}

uint8* AllocateStructMemory(const UStruct& Struct)
{
	uint8* StructMemory = static_cast<uint8*>(FMemory::Malloc(Struct.GetStructureSize()));
	RemCheckVariable(StructMemory, return {});

	Struct.InitializeStruct(StructMemory);
	
	return StructMemory;
}

void FreeStructMemory(const UStruct& Struct, uint8& Memory)
{
	Struct.DestroyStruct(&Memory);
	FMemory::Free(&Memory);
}

FString TagToStringWithoutDot(const FGameplayTag& Tag)
{
	return Tag.GetTagName().ToString().Replace(TEXTVIEW(".").GetData(), TEXTVIEW("").GetData());
}

FName TagToNameWithoutDot(const FGameplayTag& Tag)
{
	return FName{TagToStringWithoutDot(Tag)};
}

}
