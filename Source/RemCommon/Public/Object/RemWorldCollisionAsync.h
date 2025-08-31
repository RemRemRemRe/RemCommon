// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

class UObject;
struct FOverlapDatum;
struct FTraceHandle;
struct FTraceDatum;
class AActor;
class UPrimitiveComponent;

#define REM_API REMCOMMON_API

namespace Rem::Collision
{

    /**
     * Get trace data of CURRENT FRAME
     */
    REM_API FTraceDatum* QueryTraceDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle);

    /**
     * Get trace data of PREVIOUS FRAME
     */
    REM_API FTraceDatum* QueryTraceDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle);

    /**
     * Get overlap data of CURRENT FRAME
     */
    REM_API FOverlapDatum* QueryOverlapDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle);

    /**
     * Get overlap data of PREVIOUS FRAME
     */
    REM_API FOverlapDatum* QueryOverlapDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle);

    
    REM_API FOverlapDatum* QueryOverlapDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle);

    template<typename TTraceType>
    REM_API AActor* GetTraceActor(const TTraceType& TraceData);

    template<typename TTraceType>
    REM_API UPrimitiveComponent* GetTracePrimitive(const TTraceType& TraceData);
}

#undef REM_API
