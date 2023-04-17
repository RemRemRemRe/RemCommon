
#include "RemMisc.h"

#include "Macro/RemAssertionMacros.h"

namespace Rem::Common
{
	bool IsClassDefaultObject(const UObject* Object)
	{
		CheckPointer(Object, {});
		
		const UClass* Class = Object->GetClass();
		return Class->GetDefaultObject(false) == Object;
	}

	FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath)
	{
		const FString& SubPathString = SoftObjectPath.GetSubPathString();

		// may just construct a FCoreRedirectObjectName to help doing this,
		// but will that did too much extra work?
		if (int32 ObjectNameIndex;
			SubPathString.FindLastChar(TEXT('.'), ObjectNameIndex))
		{
			// + 1 to get rid of the '.'
			return SubPathString.RightChop(ObjectNameIndex + 1);
		}
		
		return {};
	}
}
