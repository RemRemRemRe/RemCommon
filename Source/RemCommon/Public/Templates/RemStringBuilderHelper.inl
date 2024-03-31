// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Templates/RemIsInstance.h"

namespace Rem
{

	template <typename StringBuilder, typename T>
	requires std::is_base_of_v<TStringBuilderBase<typename StringBuilder::ElementType>, StringBuilder>
	typename StringBuilder::BuilderType& AppendValueString(StringBuilder& Builder, const T& Value)
	{
		if constexpr (is_instance_v<T, UE::Math::TVector>)
		{
			return Builder.Appendf(TEXT("X: %.2f Y: %.2f Z: %.2f"), Value.X, Value.Y, Value.Z);
		}
		else if constexpr (is_instance_v<T, UE::Math::TRotator>)
		{
			return Builder.Appendf(TEXT("P: %.2f Y: %.2f R: %.2f"), Value.Pitch, Value.Yaw, Value.Roll);
		}
		else if constexpr (is_instance_v<T, UE::Math::TVector2>)
		{
			return Builder.Appendf(TEXT("X: %.2f Y: %.2f"), Value.X, Value.Y);
		}
		else if constexpr (is_instance_v<T, UE::Math::TVector4> || is_instance_v<T, UE::Math::TQuat> || is_instance_v<T, UE::Math::TPlane>)
		{
			return Builder.Appendf(TEXT("X: %.2f Y: %.2f Z: %.2f W: %.2f"), Value.X, Value.Y, Value.Z, Value.W);
		}
		else
		{
			static_assert(std::_Always_false<T>, "T is not supported");
			return {};
		}
	}
}
