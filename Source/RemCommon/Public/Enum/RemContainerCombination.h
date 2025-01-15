// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "HAL/Platform.h"

#include "Templates/RemEnumClassBitOperation.h"

namespace Rem::Enum
{
	enum class REMCOMMON_API EContainerCombination : uint8
	{
		None					= 0,
		NotAContainer			= None,
		ContainerItself			= None,
		ContainerItselfAsNone	= None,
		
		//// Single
		///
		Array				= 1 << 1,
		Set					= 1 << 2,
		
		MapKey				= 1 << 3,
		MapValue			= 1 << 4,
		Map					= MapKey	| MapValue,

		Struct				= 1 << 5,

		//// Double
		///
		ArrayAndSet 		= Array		| Set,
		
		ArrayAndMap 		= Array		| Map,
		ArrayAndMapKey		= Array		| MapKey,
		ArrayAndMapValue	= Array		| MapValue,
		
		SetAndMap			= Set		| Map,
		SetAndMapKey		= Set		| MapKey,
		SetAndMapValue		= Set		| MapValue,

		//// Triple
		///
		ArraySetAndMap		= Array 	| SetAndMap,
		ArraySetAndMapKey	= Array 	| SetAndMapKey,
		ArraySetAndMapValue	= Array 	| SetAndMapValue,
	};
	
	template<>
	struct BitOperation::TEnumClassBitOperationTraits<EContainerCombination> { using type = std::true_type;	};
}
