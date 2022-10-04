#pragma once

namespace Common::PropertyHelper
{
	using FIteratePropertiesFunction =
		TFunctionRef<void(const FProperty* InProperty, const void* InContainer, int32 StaticIndex,
			const FString& TypeString, const FString& KeyString, const FString& SizeString, int32 Id, int32 ParentId)>;
	
	template<typename PropertyType>
	typename TEnableIf<TIsDerivedFrom<PropertyType, FProperty>::Value, void>::
	Type IteratePropertiesOfTypeRecursive(FProperty* InProperty, const void* InContainer, const FString& InKey, FIteratePropertiesFunction InFunction, int32& InId, int32 InParentId)
	{
		// Handle container properties
		if (const FArrayProperty* ArrayProperty = CastField<FArrayProperty>(InProperty))
		{
			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

			int32 ArrayRootId = ++InId;
			
			if constexpr (TOr<	TIsSame<PropertyType, FArrayProperty>,
								TIsSame<PropertyType, FProperty>>::Value)
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

			if constexpr (TOr<	TIsSame<PropertyType, FMapProperty>,
								TIsSame<PropertyType, FProperty>>::Value)
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
					
					if constexpr (TOr<	TIsSame<PropertyType, FMapProperty>,
										TIsSame<PropertyType, FProperty>>::Value)
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

			if constexpr (TOr<	TIsSame<PropertyType, FSetProperty>,
								TIsSame<PropertyType, FProperty>>::Value)
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

			if constexpr (TOr<	TIsSame<PropertyType, FStructProperty>,
								TIsSame<PropertyType, FProperty>>::Value)
			{
				InFunction(InProperty, InContainer, 0, InProperty->GetCPPType(), InKey, TEXT("{...}"), StructRootId, InParentId);
			}

			// Recurse
			const void* StructContainer = StructProperty->ContainerPtrToValuePtr<const void>(InContainer);
			for(TFieldIterator<FProperty> It(StructProperty->Struct); It; ++It)
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
	
	template<typename PropertyType>
	typename TEnableIf<TIsDerivedFrom<PropertyType, FProperty>::Value, void>::
	Type IteratePropertiesOfType(const UStruct* InStruct, const void* InContainer, const FIteratePropertiesFunction InFunction)
	{
		int32 Id = INDEX_NONE;
		for(TFieldIterator<FProperty> It(InStruct); It; ++It)
		{
			IteratePropertiesOfTypeRecursive<PropertyType>(*It, InContainer, It->GetName(), InFunction, Id, INDEX_NONE);
		}
	}

	template<typename T = FObjectPropertyBase>
	bool IsPropertyClassChildOf(const FProperty* PropertyToCheck, const UStruct* SomeBase)
	{
		const T* ObjectProperty = CastField<T>(PropertyToCheck);
			
		return ObjectProperty && ObjectProperty->PropertyClass->IsChildOf(SomeBase);
	}
	
}
