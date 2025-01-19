// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "RemMetaTags.generated.h"

/**
 * @brief Base class for "Meta Tags" classes, use meta tag to query information from UUniversalDataAsset
 *
 *		It server for two main purposes:
 *			1. Avoid hard coded tags in c++
 *			2. Unified all tag configuration to Editor
 *
 *		If "constexpr FGameplayTag" is supported someday, we may rethink it to put tags to c++ again
 */
UCLASS(Abstract, config = RemMetaTags, defaultconfig, notplaceable, BlueprintType)
class REMCOMMON_API URemMetaTags : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	URemMetaTags(const FObjectInitializer& ObjectInitializer);
};
