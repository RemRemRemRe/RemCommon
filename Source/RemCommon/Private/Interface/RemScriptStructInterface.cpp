// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Interface/RemScriptStructInterface.h"

#include "RemCommonLog.h"
#include "Macro/RemLogMacros.h"
#include "Object/RemEmptyStruct.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemScriptStructInterface)

UScriptStruct& IRemScriptStructInterface::GetScriptStruct() const
{
	REM_LOG_FUNCTION(LogRemCommon, Error, TEXT("interface is not implemented, address:{0}"), Rem::PointerToString(this));

	return *FRemEmptyStruct::StaticStruct();
}
