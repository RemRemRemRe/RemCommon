// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

struct FGameplayTag;

namespace Rem::Test
{
/**
 * Shared test helper: registers a native gameplay tag, or returns the existing
 * tag when it is already registered in this process.
 */
[[nodiscard]] REMCOMMONTEST_API FGameplayTag AddNativeTag(FName TagName);
}
