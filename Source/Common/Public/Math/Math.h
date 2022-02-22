//

#pragma once

namespace Common
{

class FMath
{
public:
	/**
	 * @return minimal number of bits needed to represent a given integer
	 */
	COMMON_API FORCEINLINE UE_CONSTEVAL static uint8 GetBitsNeeded(uint64 Integer)
	{
		uint8 Bits = 0;
		for (uint8 BitsStep = 32; BitsStep > 0; BitsStep >>= 1)
		{
			if (Integer >> BitsStep != 0)
			{
				Bits += BitsStep;
				Integer >>= BitsStep;
			}
		}
		return Bits + Integer;
	}

};

}
