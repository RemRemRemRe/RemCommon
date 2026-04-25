// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "StructUtils/InstancedStruct.h"
#include "RemNotNull.h"
#include "Templates/CopyQualifiersFromTo.h"

namespace Rem::Property
{
namespace Private
{
template <CFProperty PropertyType, typename TVoid, typename TFunctor>
    requires std::is_void_v<TVoid>
void IteratePropertiesOfTypeRecursive(const TNotNull<const FProperty*> InProperty, const TNotNull<TVoid*> InContainer,
    TFunctor InFunction)
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

        for (auto DynamicIndex = 0; DynamicIndex < Helper.Num(); ++DynamicIndex)
        {
            TVoid* ValuePtr = Helper.GetRawPtr(DynamicIndex);
            IteratePropertiesOfTypeRecursive<PropertyType, TVoid, TFunctor>(ArrayProperty->Inner, ValuePtr, InFunction);
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

        for (FScriptMapHelper::FIterator It(Helper); It; ++It)
        {
            TVoid* PairPtr = Helper.GetPairPtr(It);
            IteratePropertiesOfTypeRecursive<PropertyType, TVoid, TFunctor>(MapProperty->KeyProp, PairPtr, InFunction);
            IteratePropertiesOfTypeRecursive<
                PropertyType, TVoid, TFunctor>(MapProperty->ValueProp, PairPtr, InFunction);
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

        for (FScriptSetHelper::FIterator It(Helper); It; ++It)
        {
            TVoid* ValuePtr = Helper.GetElementPtr(It);
            IteratePropertiesOfTypeRecursive<PropertyType, TVoid, TFunctor>(SetProperty->ElementProp, ValuePtr,
                InFunction);
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
            IteratePropertiesOfTypeRecursive<PropertyType, TVoid, TFunctor>(*It, StructContainer, InFunction);
        }
    }
    else
    {
        if constexpr (std::is_same_v<PropertyType, FProperty>)
        {
            for (auto StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
            {
                TVoid* ValuePtr = InProperty->ContainerPtrToValuePtr<TVoid>(InContainer, StaticIndex);
                InFunction(*InProperty, ValuePtr);
            }
        }
        else
        {
            if (const auto* TargetProperty = CastField<PropertyType>(InProperty))
            {
                for (auto StaticIndex = 0; StaticIndex != InProperty->ArrayDim; ++StaticIndex)
                {
                    TVoid* ValuePtr = InProperty->ContainerPtrToValuePtr<TVoid>(InContainer, StaticIndex);
                    InFunction(*TargetProperty, ValuePtr);
                }
            }
        }
    }
}
}

template <CFProperty PropertyType, typename TVoid, typename TFunctor>
    requires std::is_void_v<TVoid>
void IteratePropertiesOfType(const TNotNull<const UStruct*> InStruct, const TNotNull<TVoid*> InContainer,
    TFunctor InFunction)
{
    // type needs to be FProperty, to handle container properties
    for (TFieldIterator<FProperty> It(InStruct); It; ++It)
    {
        Private::IteratePropertiesOfTypeRecursive<PropertyType, TVoid, TFunctor>(*It, InContainer, InFunction);
    }
}

template <CFProperty PropertyType, typename TObject, typename TFunctor>
    requires Rem::CUObject<std::remove_cvref_t<TObject>> || Rem::CHasStaticStruct<std::remove_cvref_t<TObject>>
void IteratePropertiesOfType(const TNotNull<const UStruct*> Struct, const TNotNull<TObject*> Object,
    TFunctor InFunction)
{
    using TVoid = TCopyQualifiersFromTo_T<TObject, void>;

    IteratePropertiesOfType<PropertyType, TVoid, TFunctor>(Struct, static_cast<TVoid*>(Object),
        InFunction);
}


}
