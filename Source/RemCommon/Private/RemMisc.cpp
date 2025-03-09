﻿// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "RemMisc.h"

#include "GameplayTagContainer.h"
#include "Curves/RealCurve.h"
#include "HAL/IConsoleManager.h"
#include "Macro/RemAssertionMacros.h"
#include "Math/RemMath.h"
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
	// but will that do too much extra work?
	if (int32 ObjectNameIndex;
		SubPathString.FindLastChar(TEXT('.'), ObjectNameIndex))
	{
		// + 1 to get rid of the '.'
		return SubPathString.RightChop(ObjectNameIndex + 1);
	}

	return {};
}

FString PointerToString(const void* Value)
{
	return FString::Printf(TEXT("%p"), Value);
}

FString ToString(const UScriptStruct& ScriptStruct, const void* Value)
{
	FString HumanReadableMessage;
	ScriptStruct.ExportText(/*out*/ HumanReadableMessage, Value,
		/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
	return HumanReadableMessage;
}

void AppendCharRepeated(FString& String, const TCHAR Char, const int32 TimesToRepeat)
{
	String.Reserve(String.Len() + TimesToRepeat);
	for (int32 Counter = 0; Counter < TimesToRepeat; ++Counter)
	{
		String.AppendChar(Char);
	}
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

IConsoleVariable* FindConsoleVariable(const TCHAR* Name)
{
	return IConsoleManager::Get().FindConsoleVariable(Name);
}

float EvaluateCurve01(const FRealCurve& RichCurve, const float Alpha)
{
	float Min;
	float Max;
	RichCurve.GetTimeRange(Min, Max);

	const auto MappedTime = FMath::Lerp(Min, Max, Alpha);
	return RichCurve.Eval(MappedTime);
}

float EvaluateCurve01Clamped(const FRealCurve& RichCurve, const float Alpha)
{
	return Math::Clamp01(EvaluateCurve01(RichCurve, Alpha));
}

}
