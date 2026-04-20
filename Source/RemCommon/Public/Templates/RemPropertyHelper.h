// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

namespace Rem::Property
{

template <typename T = FObjectPropertyBase>
bool IsPropertyClassChildOf(const TNotNull<const FProperty*> PropertyToCheck,
    const TNotNull<const UStruct*> SomeBase)
{
    const T* ObjectProperty = CastField<T>(PropertyToCheck);

    return ObjectProperty && ObjectProperty->PropertyClass->IsChildOf(SomeBase);
}

}
