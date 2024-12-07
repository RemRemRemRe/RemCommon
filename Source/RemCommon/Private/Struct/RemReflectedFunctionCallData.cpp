// Copyright RemRemRemRe. 2024. All Rights Reserved.


#include "Struct/RemReflectedFunctionCallData.h"

#include "RemCommonLog.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemLogMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemReflectedFunctionCallData)

void FRemReflectedFunctionCallData::Execute()
{
	UFunction* LocalFunction{FunctionData.GetFunction()};
	RemCheckVariable(LocalFunction, return;);

	UObject* LocalObject;
	if (ContextObject)
	{
		LocalObject = ContextObject;

		// TODO: if this is required?
		const auto* Class = ContextObject.GetClass();
		RemCheckCondition(Class->IsChildOf(FunctionData.FunctionOwnerClass), return;, LogRemCommon, Error,
			TEXT("class of ContextObject is different from FunctionOwnerClass"));
	}
	else
	{
		RemCheckCondition(LocalFunction->HasAllFunctionFlags(EFunctionFlags::FUNC_Static), return;, LogRemCommon, Error,
			TEXT("ContextObject is required for member function"));

		LocalObject = GetMutableDefault<UObject>(FunctionData.FunctionOwnerClass);
	}

	LocalObject->ProcessEvent(LocalFunction, Parameters.GetMutableValue().GetMemory());
}

bool FRemReflectedFunctionCallData::TryFillParameters()
{
	Parameters.Reset();

	if (!FunctionData.FunctionOwnerClass || FunctionData.FunctionName.IsNone())
	{
		return false;
	}

	auto* Function = FunctionData.FunctionOwnerClass->FindFunctionByName(FunctionData.FunctionName);
	RemCheckVariable(Function, return false;);

	TArray<FPropertyBagPropertyDesc> BagPropertyDesc;

	for (TFieldIterator<FProperty> Iter{Function}; Iter && Iter->PropertyFlags & CPF_Parm; ++Iter)
	{
		auto* Property = *Iter;
		RemCheckVariable(Property, return false;);

		FPropertyBagPropertyDesc Desc{Property->GetFName(), Property};

#if REM_WITH_DEVELOPMENT_ONLY_CODE

		if (Desc.ValueType == EPropertyBagPropertyType::None)
		{
			REM_LOG_FUNCTION(LogRemCommon, Error, TEXT("parameter type:{0} of function:{1} is not supported"), Desc, FunctionData);

			FunctionData.FunctionName = {};
			return false;
		}

#endif

		BagPropertyDesc.Add(Desc);
	}

	Parameters.AddProperties(BagPropertyDesc);
	return true;
}
