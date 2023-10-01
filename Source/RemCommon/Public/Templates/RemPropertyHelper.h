#pragma once

namespace Rem::Common::PropertyHelper
{

	template<typename T = FObjectPropertyBase>
	bool IsPropertyClassChildOf(const FProperty* PropertyToCheck, const UStruct* SomeBase)
	{
		const T* ObjectProperty = CastField<T>(PropertyToCheck);
				
		return ObjectProperty && ObjectProperty->PropertyClass->IsChildOf(SomeBase);
	}

}
