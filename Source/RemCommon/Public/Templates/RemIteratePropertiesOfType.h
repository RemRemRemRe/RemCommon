﻿#pragma once

namespace Rem::Common::PropertyHelper
{
	using FIteratePropertiesFunction =
		TFunctionRef<void(const FProperty* InProperty, const void* InContainer, int32 StaticIndex,
			const FString& TypeString, const FString& KeyString, const FString& SizeString, int32 Id, int32 ParentId)>;
	
	template<typename PropertyType, typename ContainerStructType = nullptr_t>
	requires std::is_base_of_v<FProperty, PropertyType>
	void IteratePropertiesOfTypeRecursive(FProperty* InProperty, const void* InContainer, const FString& InKey, FIteratePropertiesFunction InFunction, int32& InId, int32 InParentId)
	{
		// Handle container properties
		if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(InProperty))
		{
			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

			int32 ArrayRootId = ++InId;
			
			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FArrayProperty>, std::is_same<PropertyType, FProperty>>)
			{
				FString SizeString = FString::Printf(TEXT("{Num = %d}"), Helper.Num());
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, SizeString, ArrayRootId, InParentId);
			}
			
			for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
			{
				const void* ValuePtr = Helper.GetRawPtr(DynamicIndex);
				FString KeyString = FString::Printf(TEXT("[%d]"), DynamicIndex);

				IteratePropertiesOfTypeRecursive<PropertyType>(ArrayProperty->Inner, ValuePtr, KeyString, InFunction, InId, ArrayRootId);
			}
		}
		else if (const FMapProperty* MapProperty = CastField<FMapProperty>(InProperty))
		{
			FScriptMapHelper_InContainer Helper(MapProperty, InContainer);

			int32 MapRootId = ++InId;

			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FMapProperty>, std::is_same<PropertyType, FProperty>>)
			{
				FString SizeString = FString::Printf(TEXT("{Num = %d}"), Helper.Num());
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, SizeString, MapRootId, InParentId);
			}

			int32 Num = Helper.Num();
			int32 MapIndex = 0;
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					int32 MapEntryId = ++InId;
					
					if constexpr (std::disjunction_v<
						std::is_same<PropertyType, FMapProperty>, std::is_same<PropertyType, FProperty>>)
					{
						FString KeyString = FString::Printf(TEXT("[%d]"), MapIndex++);
						FString TypeString = FString::Printf(TEXT("{%s, %s}"), *MapProperty->KeyProp->GetCPPType(), *MapProperty->ValueProp->GetCPPType());
						InFunction(InProperty, InContainer, 0, TypeString, KeyString, TEXT("{...}"), MapEntryId, MapRootId);
					}

					const void* PairPtr = Helper.GetPairPtr(DynamicIndex);
					
					IteratePropertiesOfTypeRecursive<PropertyType>(MapProperty->KeyProp, PairPtr, MapProperty->KeyProp->GetName(), InFunction, InId, MapEntryId);
					
					IteratePropertiesOfTypeRecursive<PropertyType>(MapProperty->ValueProp, PairPtr, MapProperty->ValueProp->GetName(), InFunction, InId, MapEntryId);

					--Num;
				}
			}
		}
		else if (const FSetProperty* SetProperty = CastField<FSetProperty>(InProperty))
		{
			FScriptSetHelper_InContainer Helper(SetProperty, InContainer);

			int32 SetRootId = ++InId;

			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FSetProperty>, std::is_same<PropertyType, FProperty>>)
			{
				FString SizeString = FString::Printf(TEXT("{Num = %d}"), Helper.Num());
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, SizeString, SetRootId, InParentId);
			}

			int32 Num = Helper.Num();
			int32 SetIndex = 0;
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					const void* ValuePtr = Helper.GetElementPtr(DynamicIndex);
					FString KeyString = FString::Printf(TEXT("[%d]"), SetIndex++);

					IteratePropertiesOfTypeRecursive<PropertyType>(SetProperty->ElementProp, ValuePtr, KeyString, InFunction, InId, SetRootId);

					--Num;
				}
			}
		}
		else if (const FStructProperty* StructProperty = CastField<FStructProperty>(InProperty))
		{
			int32 StructRootId = ++InId;

			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FStructProperty>, std::is_same<PropertyType, FProperty>>)
			{
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, TEXT("{...}"), StructRootId, InParentId);
			}

			const void* StructContainer = nullptr;
			const UScriptStruct* ScriptStruct = StructProperty->Struct;
			
			if constexpr (!std::is_same_v<nullptr_t, ContainerStructType>)
			{
				if (ContainerStructType::StaticStruct() == ScriptStruct)
				{
					if (const auto* InstancedStruct = StructProperty->ContainerPtrToValuePtr<ContainerStructType>(InContainer))
					{
						StructContainer = InstancedStruct->GetMemory();
						ScriptStruct = InstancedStruct->GetScriptStruct();
					}
				}
			}
			else
			{
				StructContainer = StructProperty->ContainerPtrToValuePtr<void>(InContainer);
			}
			for(TFieldIterator<FProperty> It(ScriptStruct); It; ++It)
			{
				IteratePropertiesOfTypeRecursive<PropertyType>(*It, StructContainer, It->GetName(), InFunction, InId, StructRootId);
			}
		}
		else if (InProperty->IsA<PropertyType>())
		{
			// Normal property
			int32 PropertyParentId = InParentId;
			if(InProperty->ArrayDim > 1)
			{
				// Handle static array header
				PropertyParentId = ++InId;
				FString SizeString = FString::Printf(TEXT("{Num = %d}"), InProperty->ArrayDim);
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, SizeString, PropertyParentId, InParentId);
			}

			for (int32 StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
			{
				const void* ValuePtr = InProperty->ContainerPtrToValuePtr<const void>(InContainer, StaticIndex);

				FString KeyString = InProperty->ArrayDim == 1 ? InKey : FString::Printf(TEXT("[%d]"), StaticIndex);
				FString ValueString;
				InProperty->ExportText_Direct(ValueString, ValuePtr, ValuePtr, nullptr, PPF_None);

				InFunction(InProperty, InContainer, StaticIndex, InProperty->GetCPPType(), KeyString, ValueString, ++InId, PropertyParentId);
			}
		}
	}
	
	template<typename PropertyType, typename ContainerStructType = nullptr_t>
	requires std::is_base_of_v<FProperty, PropertyType>
	void IteratePropertiesOfType(const UStruct* InStruct, const void* InContainer, const FIteratePropertiesFunction InFunction)
	{
		int32 Id = INDEX_NONE;
		for(TFieldIterator<FProperty> It(InStruct); It; ++It)
		{
			IteratePropertiesOfTypeRecursive<PropertyType, ContainerStructType>(*It, InContainer, It->GetName(), InFunction, Id, INDEX_NONE);
		}
	}

}