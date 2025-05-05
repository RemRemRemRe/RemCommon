// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Object/RemWorldCollisionAsync.h"

#include "WorldCollision.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemPrivateMemberAccessor.h"

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

}

namespace Rem::Collision
{

FTraceDatum* QueryTraceData(const UObject& WorldContext, const FTraceHandle& Handle)
{
	RemCheckVariable(Handle, return {});

	auto* World = WorldContext.GetWorld();
	RemCheckVariable(World, return {});

	auto& AsyncTrace = GWorldAsyncTraceStateAccessor::Access(*World);
	auto& DataBuffer = AsyncTrace.GetBufferForCurrentFrame();
	if (auto* Data = FBufferIndexPair(Handle._Data.Index).DatumLookup(DataBuffer.TraceData))
	{
		return Data;
	}

	return nullptr;
}

FOverlapDatum* QueryOverlapData(const UObject& WorldContext, const FTraceHandle& Handle)
{
	RemCheckVariable(Handle, return {});

	auto* World = WorldContext.GetWorld();
	RemCheckVariable(World, return {});

	auto& AsyncTrace = GWorldAsyncTraceStateAccessor::Access(*World);
	auto& DataBuffer = AsyncTrace.GetBufferForCurrentFrame();
	if (auto* Data = FBufferIndexPair(Handle._Data.Index).DatumLookup(DataBuffer.OverlapData))
	{
		return Data;
	}

	return nullptr;
}

}
