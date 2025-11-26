// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Templates/RemIsInstance.h"

namespace Rem
{

	template <typename StringBuilder, typename T>
	requires std::derived_from<StringBuilder, TStringBuilderBase<typename StringBuilder::ElementType>>
    StringBuilder::BuilderType& AppendValueString(StringBuilder& Builder, const T& Value)
	{
		if constexpr (is_instance_v<T, UE::Math::TVector>)
		{
			return Builder.Appendf(TEXT("X:%.3f Y:%.3f Z:%.3f"), Value.X, Value.Y, Value.Z);
		}
		else if constexpr (is_instance_v<T, UE::Math::TRotator>)
		{
			return Builder.Appendf(TEXT("P:%.3f Y:%.3f R:%.3f"), Value.Pitch, Value.Yaw, Value.Roll);
		}
		else if constexpr (is_instance_v<T, UE::Math::TVector2>)
		{
			return Builder.Appendf(TEXT("X:%.3f Y:%.3f"), Value.X, Value.Y);
		}
		else if constexpr (is_instance_v<T, UE::Math::TVector4> || is_instance_v<T, UE::Math::TQuat> || is_instance_v<T, UE::Math::TPlane>)
		{
			return Builder.Appendf(TEXT("X:%.3f Y:%.3f Z:%.3f W:%.3f"), Value.X, Value.Y, Value.Z, Value.W);
		}
		else if constexpr (is_instance_v<T, UE::Math::TTransform>)
		{
		    const auto Rotator(Value.Rotator());
		    const auto Translation(Value.GetTranslation());
		    const auto Scale3D(Value.GetScale3D());

            if (Scale3D.Equals(decltype(Scale3D)::OneVector))
            {
		        return Builder.Appendf(TEXT("X:%.3f Y:%.3f Z:%.3f # P:%.2f Y:%.2f R:%.2f"),
		            Translation.X, Translation.Y, Translation.Z, Rotator.Pitch, Rotator.Yaw, Rotator.Roll);
            }
		    return Builder.Appendf(TEXT("X:%.3f Y:%.3f Z:%.3f # P:%.2f Y:%.2f R:%.2f # SX:%.3f SY:%.3f SZ:%.3f"),
		        Translation.X, Translation.Y, Translation.Z, Rotator.Pitch, Rotator.Yaw, Rotator.Roll, Scale3D.X, Scale3D.Y, Scale3D.Z);
		}
		else
		{
			static_assert(always_false<T>::value, "T is not supported");
			return {};
		}
	}
}
