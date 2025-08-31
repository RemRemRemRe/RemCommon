// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Object/RemWorldCollisionAsync.h"

#include "WorldCollision.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemPrivateMemberAccessor.h"
#include "Enum/RemHelperEnum.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

#define REM_API REMCOMMON_API

namespace
{

//////// copy from WorldCollisionAsync.cpp

/** For referencing a thread data buffer and a datum within it */
struct FBufferIndexPair
{
	int32 Block;
	int32 Index;

	explicit FBufferIndexPair(const int32 InVal)
		: Block(InVal / ASYNC_TRACE_BUFFER_SIZE)
		, Index(InVal % ASYNC_TRACE_BUFFER_SIZE)
	{
	}

	FBufferIndexPair(const int32 InBlock, const int32 InIndex)
		: Block(InBlock)
		, Index(InIndex)
	{
	}

	template <typename DatumType>
	DatumType* DatumLookup(TArray<TUniquePtr<TTraceThreadData<DatumType>>>& Array) const
	{
		// if not valid index, return
		if (!Array.IsValidIndex(Block))
		{
			return nullptr;
		}

		if (Index < 0 || Index >= ASYNC_TRACE_BUFFER_SIZE)
		{
			return nullptr;
		}

		return &Array[Block]->Buffer[Index];
	}

	template <typename DatumType>
	FORCEINLINE DatumType& DatumLookupChecked(TArray<TUniquePtr<TTraceThreadData<DatumType>>>& Array) const
	{
		check(Index >= 0 && Index < ASYNC_TRACE_BUFFER_SIZE);
		return Array[Block]->Buffer[Index];
	}
};

REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(GWorldAsyncTraceStateAccessor, &UWorld::AsyncTraceState, FWorldAsyncTraceState UWorld::*);

using EUseCurrentFrameBuffer = Rem::Enum::EYesOrNo;
using EQueryTraceData = Rem::Enum::EYesOrNo;

template <EUseCurrentFrameBuffer UseCurrentFrameBuffer = EUseCurrentFrameBuffer::Yes,
	EQueryTraceData QueryTraceData = EQueryTraceData::Yes>
std::conditional_t<QueryTraceData == EQueryTraceData::Yes, FTraceDatum*, FOverlapDatum*>
	QueryDataInternal(const UObject& WorldContext, const FTraceHandle& Handle)
{
	RemCheckVariable(Handle, return {});

	auto* World = WorldContext.GetWorld();
	RemCheckVariable(World, return {});

	auto& AsyncTrace = GWorldAsyncTraceStateAccessor::Access(*World);

	AsyncTraceData* DataBuffer;
	if constexpr (UseCurrentFrameBuffer == EUseCurrentFrameBuffer::Yes)
	{
		DataBuffer = &AsyncTrace.GetBufferForCurrentFrame();
	}
	else
	{
		DataBuffer = &AsyncTrace.GetBufferForPreviousFrame();
	}

	if constexpr (QueryTraceData == EQueryTraceData::Yes)
	{
		if (auto* Data = FBufferIndexPair(Handle._Data.Index).DatumLookup(DataBuffer->TraceData))
		{
			return Data;
		}
	}
	else
	{
		if (auto* Data = FBufferIndexPair(Handle._Data.Index).DatumLookup(DataBuffer->OverlapData))
		{
			return Data;
		}
	}

	return {};
}

}

namespace Rem::Collision
{

FTraceDatum* QueryTraceDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle)
{
	return QueryDataInternal<EUseCurrentFrameBuffer::Yes, EQueryTraceData::Yes>(WorldContext, Handle);
}

FTraceDatum* QueryTraceDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle)
{
	return QueryDataInternal<EUseCurrentFrameBuffer::No, EQueryTraceData::Yes>(WorldContext, Handle);
}

FOverlapDatum* QueryOverlapDataCurrentFrame(const UObject& WorldContext, const FTraceHandle& Handle)
{
	return QueryDataInternal<EUseCurrentFrameBuffer::Yes, EQueryTraceData::No>(WorldContext, Handle);
}

FOverlapDatum* QueryOverlapDataPreviousFrame(const UObject& WorldContext, const FTraceHandle& Handle)
{
	return QueryDataInternal<EUseCurrentFrameBuffer::No, EQueryTraceData::No>(WorldContext, Handle);
}

template <typename TTraceType>
REM_API AActor* GetTraceActor(const TTraceType& TraceData)
{
    return TraceData.GetActor();
}

template REM_API AActor* GetTraceActor<FOverlapResult>(const FOverlapResult& TraceData);
template REM_API AActor* GetTraceActor<FHitResult>(const FHitResult& TraceData);

template <typename TTraceType>
REM_API UPrimitiveComponent* GetTracePrimitive(const TTraceType& TraceData)
{
    return TraceData.GetComponent();
}

template REM_API UPrimitiveComponent* GetTracePrimitive<FOverlapResult>(const FOverlapResult& TraceData);
template REM_API UPrimitiveComponent* GetTracePrimitive<FHitResult>(const FHitResult& TraceData);

}

#undef REM_API
