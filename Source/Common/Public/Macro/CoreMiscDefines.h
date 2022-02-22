// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// An alternative for PURE_VIRTUAL which will cause unwanted process terminated
#define VIRTUAL_WARN(CategoryName, func, ...) { UE_LOG(CategoryName, Warning, TEXT("Called a virtual function which is not implemented (%s)"), TEXT(#func)); __VA_ARGS__ }