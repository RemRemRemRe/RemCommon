// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"

namespace Rem::Property
{
namespace Private
{
	template<Concepts::is_property PropertyType, typename TVoid, typename TFunctor>
    requires std::is_void_v<TVoid>
	void IteratePropertiesOfTypeRecursive(const FProperty* InProperty, TVoid* InContainer, TFunctor InFunction)
	{
		// Handle container properties
		if (const auto* ArrayProperty = CastField<FArrayProperty>(InProperty))
		{
			FScriptArrayHelper_InContainer Helper(ArrayProperty, InContainer);

			// this property type or any property type
			if constexpr (std::is_same_v<PropertyType, FArrayProperty>)
			{
				InFunction(*ArrayProperty, InContainer);
			}
			else if constexpr (std::is_same_v<PropertyType, FProperty>)
			{
				InFunction(*InProperty, InContainer);
			}

			for (int32 DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
			{
				TVoid* ValuePtr = Helper.GetRawPtr(DynamicIndex);
				IteratePropertiesOfTypeRecursive<PropertyType>(ArrayProperty->Inner, ValuePtr, InFunction);
			}
		}
		else if (const auto* MapProperty = CastField<FMapProperty>(InProperty))
		{
			FScriptMapHelper_InContainer Helper(MapProperty, InContainer);
		    
		    // this property type or any property type
		    if constexpr (std::is_same_v<PropertyType, FMapProperty>)
		    {
		        InFunction(*MapProperty, InContainer);
		    }
		    else if constexpr (std::is_same_v<PropertyType, FProperty>)
		    {
		        InFunction(*InProperty, InContainer);
		    }

			int32 Num = Helper.Num();
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					TVoid* PairPtr = Helper.GetPairPtr(DynamicIndex);
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
		    if constexpr (std::is_same_v<PropertyType, FSetProperty>)
		    {
		        InFunction(*SetProperty, InContainer);
		    }
		    else if constexpr (std::is_same_v<PropertyType, FProperty>)
		    {
		        InFunction(*InProperty, InContainer);
		    }

			int32 Num = Helper.Num();
			for (int32 DynamicIndex = 0; Num; ++DynamicIndex)
			{
				if (Helper.IsValidIndex(DynamicIndex))
				{
					TVoid* ValuePtr = Helper.GetElementPtr(DynamicIndex);
					IteratePropertiesOfTypeRecursive<PropertyType>(SetProperty->ElementProp, ValuePtr, InFunction);

					--Num;
				}
			}
		}
		else if (const auto* StructProperty = CastField<FStructProperty>(InProperty))
		{
		    // this property type or any property type
		    if constexpr (std::is_same_v<PropertyType, FStructProperty>)
		    {
		        InFunction(*StructProperty, InContainer);
		    }
		    else if constexpr (std::is_same_v<PropertyType, FProperty>)
		    {
		        InFunction(*InProperty, InContainer);
		    }

			TVoid* StructContainer;
			const UScriptStruct* ScriptStruct = StructProperty->Struct;

			if (FInstancedStruct::StaticStruct() == ScriptStruct)
			{
				// redirect it for instanced struct
				if (auto* InstancedStruct = StructProperty->ContainerPtrToValuePtr<FInstancedStruct>(InContainer))
				{
                    if constexpr (std::is_const_v<TVoid>)
                    {
					    StructContainer = static_cast<TVoid*>(InstancedStruct->GetMemory());
                    }
                    else
                    {
					    StructContainer = static_cast<TVoid*>(InstancedStruct->GetMutableMemory());
                    }
				    
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
				StructContainer = StructProperty->ContainerPtrToValuePtr<TVoid>(InContainer);
			}

			// type needs to be FProperty, to handle container properties
			for (TFieldIterator<FProperty> It(ScriptStruct); It; ++It)
			{
				IteratePropertiesOfTypeRecursive<PropertyType>(*It, StructContainer, InFunction);
			}
		}
		else
		{
		    if constexpr (std::is_same_v<PropertyType, FProperty>)
		    {
		        for (int32 StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
		        {
		            TVoid* ValuePtr = InProperty->ContainerPtrToValuePtr<TVoid>(InContainer, StaticIndex);
		            InFunction(*InProperty, ValuePtr);
		        }
		    }
		    else
		    {
		        if (const auto* TargetProperty = CastField<PropertyType>(InProperty))
		        {
			        for (int32 StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
			        {
				        TVoid* ValuePtr = InProperty->ContainerPtrToValuePtr<TVoid>(InContainer, StaticIndex);
				        InFunction(*TargetProperty, ValuePtr);
			        }
		        }
		    }
		}
	}
}
    
    template<Concepts::is_property PropertyType, typename TVoid, typename TFunctor>
    requires std::is_void_v<TVoid>
    void IteratePropertiesOfType(const UStruct* InStruct, TVoid* InContainer, TFunctor InFunction)
    {
        // type needs to be FProperty, to handle container properties
        for (TFieldIterator<FProperty> It(InStruct); It; ++It)
        {
            Private::IteratePropertiesOfTypeRecursive<PropertyType>(*It, InContainer, InFunction);
        }
    }
    
	template<Concepts::is_property PropertyType, typename TObject, typename TFunctor>
    requires Rem::Concepts::is_uobject<std::remove_cvref_t<TObject>> || Rem::Concepts::has_static_struct<std::remove_cvref_t<TObject>>
	void IteratePropertiesOfType(const UStruct* Struct, TObject&& Object, TFunctor InFunction)
	{
        if constexpr (std::is_const_v<TObject>)
        {
			IteratePropertiesOfType<PropertyType, const void, TFunctor>(Struct, static_cast<const void*>(&Object), InFunction);
        }
        else
        {
			IteratePropertiesOfType<PropertyType, void, TFunctor>(Struct, static_cast<void*>(&Object), InFunction);
        }
	}
    
    

}
