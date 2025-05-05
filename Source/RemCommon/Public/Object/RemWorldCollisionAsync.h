// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

class UObject;
struct FOverlapDatum;
struct FTraceHandle;
struct FTraceDatum;

namespace Rem::Collision
{

/**
 * Get trace data of CURRENT FRAME
 */
REMCOMMON_API FTraceDatum* QueryTraceData(const UObject& WorldContext, const FTraceHandle& Handle);

/**
 * Get overlap data of CURRENT FRAME
 */
REMCOMMON_API FOverlapDatum* QueryOverlapData(const UObject& WorldContext, const FTraceHandle& Handle);

}
