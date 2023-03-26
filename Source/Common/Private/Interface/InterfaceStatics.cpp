// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/InterfaceStatics.h"

bool UInterfaceStatics::IsValidInterface(const FScriptInterface Interface)
{
	return IsValid(Interface.GetObject());
}
