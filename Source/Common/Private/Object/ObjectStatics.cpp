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
	const TFunctionRef<void(UObject*)> Predicate)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner);
	CheckPointer(ObjectProperty, return;);
	
	FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);
	
	for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
	{
		UObject* Object = Cast<UObject>(
			ObjectProperty->GetObjectPropertyValue(Helper.GetRawPtr(DynamicIndex)));
		CheckPointer(Object, continue;);

		Predicate(Object);
	}
}

bool IsImplementedInBlueprint(const UFunction* Function)
{
	return Function && ensure(Function->GetOuter())
		&& Function->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
}
	
}
