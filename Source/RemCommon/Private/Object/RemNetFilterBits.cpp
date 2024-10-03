// Copyright RemRemRemRe, All Rights Reserved.


#include "Object/RemNetFilterBits.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/EngineTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemNetFilterBits)

bool FRemNetFilterBits::Matches(const ENetMode NetMode, const ENetRole Role) const
{
	if (NetMode == NM_Client)
	{
		if (Role == ROLE_AutonomousProxy && !bRunOnLocalPlayer)
		{
			return false;
		}
		
		if (Role == ROLE_SimulatedProxy && !bRunOnSimulatedProxy)
		{
			return false;
		}
	}
	else if (NetMode == NM_DedicatedServer && !bRunOnDedicatedServer)
	{
		return false;
	}
	else if (NetMode == NM_Standalone && !bRunOnStandalone)
	{
		return false;
	}
	else if (NetMode == NM_ListenServer && !bRunOnListenServer)
	{
		// todo net role ?
		return false;
	}

	return true;
}
