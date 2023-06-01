// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/RemStringInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemStringInterface)


FString IStringInterface::ToString() const
{
	// if you don't want to print in some case like shipping,
	// could change this behavior here
	return ToStringInternal();
}

