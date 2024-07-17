// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"

namespace Rem::Property
{
	using FIteratePropertiesFunction = TFunctionRef<void(const FProperty* InProperty, const void* InContainer)>;

namespace Impl
{
	template<Concepts::is_property PropertyType>
	void IteratePropertiesOfTypeRecursive(const FProperty* InProperty, const void* InContainer, const FIteratePropertiesFunction& InFunction)
	{
		// Handle container properties
		if (const auto* ArrayProperty = CastField<FArrayProperty>(InProperty))
		{
			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

			// this property type or any property type
			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FArrayProperty>, std::is_same<PropertyType, FProperty>>)
			{
				InFunction(InProperty, InContainer);
			}

			for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
			{
				const void* ValuePtr = Helper.GetRawPtr(DynamicIndex);
				IteratePropertiesOfTypeRecursive<PropertyType>(ArrayProperty->Inner, ValuePtr, InFunction);
			}
		}
		else if (const auto* MapProperty = CastField<FMapProperty>(InProperty))
		{
			FScriptMapHelper_InContainer Helper(MapProperty, InContainer);

			// this property type or any property type
			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FMapProperty>, std::is_same<PropertyType, FProperty>>)
			{
				InFunction(InProperty, InContainer);
			}

			int32 Num = Helper.Num();
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					const void* PairPtr = Helper.GetPairPtr(DynamicIndex);
					IteratePropertiesOfTypeRecursive<PropertyType>(MapProperty->KeyProp, PairPtr, InFunction);
					IteratePropertiesOfTypeRecursive<PropertyType>(MapProperty->ValueProp, PairPtr, InFunction);

					--Num;
				}
			}
		}
		else if (const auto* SetProperty = CastField<FSetProperty>(InProperty))
		{
			FScriptSetHelper_InContainer Helper(SetProperty, InContainer);

			// this property type or any property type
			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FSetProperty>, std::is_same<PropertyType, FProperty>>)
			{
				InFunction(InProperty, InContainer);
			}

			int32 Num = Helper.Num();
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					const void* ValuePtr = Helper.GetElementPtr(DynamicIndex);
					IteratePropertiesOfTypeRecursive<PropertyType>(SetProperty->ElementProp, ValuePtr, InFunction);

					--Num;
				}
			}
		}
		else if (const auto* StructProperty = CastField<FStructProperty>(InProperty))
		{
			// this property type or any property type
			if constexpr (std::disjunction_v<
				std::is_same<PropertyType, FStructProperty>, std::is_same<PropertyType, FProperty>>)
			{
				InFunction(InProperty, InContainer);
			}

			const void* StructContainer;
			const UScriptStruct* ScriptStruct = StructProperty->Struct;

			if (FInstancedStruct::StaticStruct() == ScriptStruct)
			{
				// redirect it for instanced struct
				if (const auto* InstancedStruct = StructProperty->ContainerPtrToValuePtr<FInstancedStruct>(InContainer))
				{
					StructContainer = InstancedStruct->GetMemory();
					ScriptStruct = InstancedStruct->GetScriptStruct();
				}
				else
				{
					// InstancedStruct is null
					return;
				}
			}
			else
			{
				StructContainer = StructProperty->ContainerPtrToValuePtr<void>(InContainer);
			}

			// type needs to be FProperty, to handle container properties
			for (TFieldIterator<FProperty> It(ScriptStruct); It; ++It)
			{
				IteratePropertiesOfTypeRecursive<PropertyType>(*It, StructContainer, InFunction);
			}
		}
		else if (InProperty->IsA<PropertyType>())
		{
			for (int32 StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
			{
				const void* ValuePtr = InProperty->ContainerPtrToValuePtr<const void>(InContainer, StaticIndex);
				InFunction(InProperty, ValuePtr);
			}
		}
	}
}

	template<Concepts::is_property PropertyType>
	void IteratePropertiesOfType(const UStruct* InStruct, const void* InContainer, const FIteratePropertiesFunction& InFunction)
	{
		// type needs to be FProperty, to handle container properties
		for(TFieldIterator<FProperty> It(InStruct); It; ++It)
		{
			Impl::IteratePropertiesOfTypeRecursive<PropertyType>(*It, InContainer, InFunction);
		}
	}

}
