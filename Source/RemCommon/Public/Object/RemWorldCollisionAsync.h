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
REMCOMMON_API FTraceDatum* QueryTraceDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle);

/**
 * Get trace data of PREVIOUS FRAME
 */
REMCOMMON_API FTraceDatum* QueryTraceDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle);

/**
 * Get overlap data of CURRENT FRAME
 */
REMCOMMON_API FOverlapDatum* QueryOverlapDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle);

/**
 * Get overlap data of PREVIOUS FRAME
 */
REMCOMMON_API FOverlapDatum* QueryOverlapDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle);

}
