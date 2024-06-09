// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemCommonWrapperTypes.h"
#include "RemConcepts.h"

namespace Rem
{
	template<Concepts::is_uobject TObject, typename TWrapper, typename TTransform>
	requires std::is_same_v<std::true_type, typename TWrapper::IsArrayOfObjectWrapper>
	void ArrayToWrapper(const TArray<TObject*>& Array, TWrapper& Wrapper, TTransform Transform)
	{
		Wrapper.GetElements().Reserve(Array.Num());
			
		Algo::Transform(Array, Wrapper.GetElements(), Transform);
	}

	template<Concepts::is_uobject TObject, typename TWrapper>
	requires std::is_same_v<std::true_type, typename TWrapper::IsArrayOfObjectWrapper>
	void ArrayToWrapper(const TArray<TObject*>& Array, TWrapper& Wrapper)
	{
		ArrayToWrapper(Array, Wrapper,
			[](UObject* Object)
		{
			return TWrapper::WrapperElementType{Object};
		});
	}

	template<Concepts::is_uobject TObject, typename TWrapper, typename TTransform>
	requires std::is_same_v<std::true_type, typename TWrapper::IsArrayOfObjectWrapper>
	void WrapperToArray(const TWrapper& Wrapper, TArray<TObject*>& Array, TTransform Transform)
	{
		Array.Reserve(Wrapper.GetElements().Num());
			
		Algo::Transform(Wrapper.GetElements(), Array, Transform);
	}

	template<Concepts::is_uobject TObject, typename TWrapper>
	requires std::is_same_v<std::true_type, typename TWrapper::IsArrayOfObjectWrapper>
	void WrapperToArray(const TWrapper& Wrapper, TArray<TObject*>& Array)
	{
		WrapperToArray(Wrapper, Array,
			[](const typename TWrapper::WrapperElementType& Object)
		{
			return Object.template GetObject<TObject>();
		});
	}


}
