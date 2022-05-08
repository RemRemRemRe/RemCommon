// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/ObjectStatics.h"
#include "Macro/AssertionMacros.h"

UObject* UObjectStatics::GetObject(const TSoftObjectPtr<> SoftObjectPtr, UClass* ObjectClass)
{
	return SoftObjectPtr.Get();
}

namespace Common::ObjectStatics
{
	
void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
	const TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner);
	CheckPointer(ObjectProperty, return;);
	
	FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);
	
	for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
	{
		uint8* PropertyValueAddress = Helper.GetRawPtr(DynamicIndex);

		Predicate(PropertyValueAddress, DynamicIndex);
	}
}

bool IsImplementedInBlueprint(const UFunction* Function)
{
	return Function && ensure(Function->GetOuter())
		&& Function->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
}
	
}
