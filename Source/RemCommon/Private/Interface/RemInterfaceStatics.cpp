﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/RemInterfaceStatics.h"

bool URemInterfaceStatics::IsValidInterface(const FScriptInterface Interface)
{
	return IsValid(Interface.GetObject());
}