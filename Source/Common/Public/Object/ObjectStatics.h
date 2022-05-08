// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ObjectStatics.generated.h"

/**
 * 
 */
UCLASS()
class COMMON_API UObjectStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = ObjectClass))
	static UObject* GetObject(TSoftObjectPtr<UObject> SoftObjectPtr, UClass* ObjectClass);
	
};

namespace Common::ObjectStatics
{
	COMMON_API void ForeachObjectInArray(const FArrayProperty* ArrayProperty, const UObject* InContainer,
		TFunctionRef<void(void* ObjectMemberPtr, int32 Index)> Predicate);

	COMMON_API bool IsImplementedInBlueprint(const UFunction* Function);
}
