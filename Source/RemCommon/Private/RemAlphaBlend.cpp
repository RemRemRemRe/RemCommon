// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "RemAlphaBlend.h"

#include "AlphaBlend.h"
#include "Curves/CurveBase.h"
#include "Curves/RichCurve.h"
#include "Macro/RemAssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemAlphaBlend)

FVector3f FRemAlphaBlendCurveAndOption::GetAlpha(const float Time, const float BlendTime,
	const EAlphaBlendOption BlendOption, const FRemCurveBaseWrapper& CurveWrapper, bool& bOutVectorAlpha)
{
	RemCheckCondition(BlendTime > 0.0f, return FVector3f::OneVector;, REM_NO_LOG_BUT_ENSURE);

	const auto Alpha = FMath::Clamp(Time / BlendTime, 0.0f, 1.0f);

	auto* Curve = CurveWrapper.Get();
	if (!Curve || BlendOption != EAlphaBlendOption::Custom)
	{
		bOutVectorAlpha = false;
		return FVector3f{FAlphaBlend::AlphaToBlendOption(Alpha, BlendOption)};
	}

    TArray<FRichCurveEditInfoConst, TInlineAllocator<4>> Curves;
    Curve->GetCurves(Curves);

	RemCheckCondition(Curves.Num() > 0, return FVector3f::OneVector;, REM_NO_LOG_BUT_ENSURE);
	bOutVectorAlpha = Curves.Num() >= 3;

	auto VectorAlpha{FVector3f::OneVector};
	for (const auto Index : {0, 1, 2})
	{
		if (Curves.Num() > Index)
		{
			VectorAlpha[Index] = Rem::EvaluateCurve01Clamped(*Curves[Index].CurveToEdit, Alpha);
		}
		else
		{
			// copy value from previous element
			VectorAlpha[Index] = VectorAlpha[Index - 1];
		}
	}

	return VectorAlpha;
}
