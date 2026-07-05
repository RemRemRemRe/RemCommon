// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "RemMisc.h"

#include "GameplayTagContainer.h"
#include "Curves/RealCurve.h"
#include "HAL/IConsoleManager.h"
#include "Macro/RemAssertionMacros.h"
#include "Math/RemMath.h"
#include "UObject/Object.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/SoftObjectPath.h"

namespace Rem
{

bool IsClassDefaultObject(const UObject* Object)
{
    RemCheckVariable(Object, {});

    const UClass* Class = Object->GetClass();
    return Class->GetDefaultObject(false) == Object;
}

uint8* AllocateStructMemory(const UStruct& Struct)
{
    auto StructMemory = static_cast<uint8*>(
        FMemory::Malloc(Struct.GetStructureSize(), Struct.GetMinAlignment()));
    RemCheckVariable(StructMemory, return {});

    Struct.InitializeStruct(StructMemory);

    return StructMemory;
}

void FreeStructMemory(const UStruct& Struct, uint8& Memory)
{
    Struct.DestroyStruct(&Memory);
    FMemory::Free(&Memory);
}

IConsoleVariable* FindConsoleVariable(const TCHAR* Name)
{
    return IConsoleManager::Get().FindConsoleVariable(Name);
}

float EvaluateCurve01(const FRealCurve& RichCurve, const float Alpha)
{
    float Min;
    float Max;
    RichCurve.GetTimeRange(Min, Max);

    const auto MappedTime = FMath::Lerp(Min, Max, Alpha);
    return RichCurve.Eval(MappedTime);
}

float EvaluateCurve01Clamped(const FRealCurve& RichCurve, const float Alpha)
{
    return Math::Clamp01(EvaluateCurve01(RichCurve, Alpha));
}

}
