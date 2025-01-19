// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "Struct/RemReflectedFunctionData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemReflectedFunctionData)

UFunction* FRemReflectedFunctionData::GetFunction() const
{
	if (!FunctionOwnerClass || FunctionName.IsNone())
	{
		return {};
	}

	// for (const UClass* Class = FunctionOwnerClass; Class; Class = Class->GetSuperClass())
	// {
		// using ExcludeSuper to avoid populate UClass::AllFunctionsCache
		if (auto* Function = FunctionOwnerClass->FindFunctionByName(FunctionName, EIncludeSuperFlag::ExcludeSuper))
		{
			return Function;
		}
	// }

	return {};
}
