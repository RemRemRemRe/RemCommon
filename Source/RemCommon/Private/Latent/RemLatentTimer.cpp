// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Latent/RemLatentTimer.h"

#include "RemCommonLog.h"
#include "Engine/World.h"
#include "Macro/RemAssertionMacros.h"
#include "Macro/RemLogMacros.h"
#include "Misc/ScopeExit.h"

namespace Rem::Latent
{

FTimerHandle FTimerHandle::NewHandle()
{
	static FHandleType LatentTimerGlobalSerialNumber{};
	++LatentTimerGlobalSerialNumber;

	if (!LIKELY(REM_ENSURE(LatentTimerGlobalSerialNumber != 0)))
	{
		REM_LOG_FUNCTION(LogRemCommon, Warning, TEXT("LatentTimerGlobalSerialNumber is wrapper to 0"));

		// it's wrapped to 0, advance it again to let it be "valid"
		++LatentTimerGlobalSerialNumber;
	}

	return FTimerHandle{LatentTimerGlobalSerialNumber};
}

FTimerLatentAction_Delay::FTimerLatentAction_Delay(const FTimerDelegate& InDelegate,
	const FTimerParameterHelper_Time& DelayParameter)
{
	Delegate = InDelegate;

	TimeOrFrameToDelay.Time = DelayParameter.TimeToDelay;
	bTimeOrFrame = 1;

	bMaxOncePerFrame = TimeOrFrameToDelay.Time <= 0.0f || DelayParameter.bMaxOncePerFrame;

	LoopCount = DelayParameter.LoopCount;

	CurrentTimeOrFrame.Time =
		DelayParameter.InitialDelay < 0.0f ? DelayParameter.TimeToDelay : DelayParameter.InitialDelay;

	bPausedOneFrame = DelayParameter.bSkipCountingThisFrame;
}

FTimerLatentAction_Delay::FTimerLatentAction_Delay(const FTimerDelegate& InDelegate,
	const FTimerParameterHelper_Frame& DelayParameter)
{
	Delegate = InDelegate;

	TimeOrFrameToDelay.Frame = DelayParameter.FrameToDelay + 1;
	//bTimeOrFrame = 0;

	LoopCount = DelayParameter.LoopCount;

	CurrentTimeOrFrame.Frame = DelayParameter.InitialDelay != std::numeric_limits<uint32>::max()
			? DelayParameter.InitialDelay + 1
			: TimeOrFrameToDelay.Frame;

	bPausedOneFrame = DelayParameter.bSkipCountingThisFrame;
}

void FTimerLatentAction_Delay::UpdateOperation(FLatentResponse& Response)
{
#pragma region check_pause_stop

	if (bStopped)
	{
		// stop now
		return Super::UpdateOperation(Response);
	}

	if (bPausedOneFrame)
	{
		bPausedOneFrame = false;
		return;
	}

	if (bPaused)
	{
		// skip processing
		return;
	}

#pragma endregion check_pause_stop

	bool bTriggered;
	bool bFinished{false};
	uint32 CallCount{1};

	ON_SCOPE_EXIT
	{
		if (bFinished)
		{
			// requested to stop, it set remove action
			Super::UpdateOperation(Response);
		}
	};

	if (bTimeOrFrame)
	{
		CurrentTimeOrFrame.Time -= Response.ElapsedTime();

		bTriggered = CurrentTimeOrFrame.Time <= 0.0f;

		// check looping when triggered
		if (bTriggered)
		{
			// loops forever
			if (LoopCount == 0)
			{
				// reset
				CurrentTimeOrFrame.Time = TimeOrFrameToDelay.Time;

#if REM_WITH_DEVELOPMENT_ONLY_CODE

				RemCheckCondition(TimeOrFrameToDelay.Time >= 0.0f, {bFinished = true; return;});

#endif

				// if Time is zero, bMaxOncePerFrame will be true
				CallCount =	bMaxOncePerFrame ? 1 : FMath::TruncToInt( Response.ElapsedTime() / TimeOrFrameToDelay.Time ) + 1;
			}
			else if (LoopCount > 1)
			{
				// reset
				CurrentTimeOrFrame.Time = TimeOrFrameToDelay.Time;

#if REM_WITH_DEVELOPMENT_ONLY_CODE

				RemCheckCondition(TimeOrFrameToDelay.Time >= 0.0f, {bFinished = true; return;});

#endif

				// if Time is zero, bMaxOncePerFrame will be true
				CallCount =	bMaxOncePerFrame ? 1 : FMath::TruncToInt( Response.ElapsedTime() / TimeOrFrameToDelay.Time ) + 1;

				if (LoopCount - CallCount >= 1)
				{
					// we have enough loops left
					LoopCount -= CallCount;
				}
				else
				{
					// clamp it
					CallCount = LoopCount;

					// run out of loops, stop it
					bFinished = true;
				}
			}
			else
			{
				// no loop, go to finished
				bFinished = true;
			}
		}
	}
	else
	{
		--CurrentTimeOrFrame.Frame;

		bTriggered = CurrentTimeOrFrame.Frame == 0;

		// check looping when triggered
		if (bTriggered)
		{
			// loops forever
			if (LoopCount == 0)
			{
				// reset, loops forever
				CurrentTimeOrFrame.Frame = TimeOrFrameToDelay.Frame;
			}
			else if (LoopCount > 1)
			{
				--LoopCount;

				// reset
				CurrentTimeOrFrame.Frame = TimeOrFrameToDelay.Frame;
			}
			else
			{
				// no loop, go to finished
				bFinished = true;
			}
		}
	}

	if (!bTriggered)
	{
		return;
	}

	while (CallCount > 0)
	{
		if (Delegate.IsBound())
		{
			--CallCount;

			// may change internal state
			Delegate.Execute();
		}
		else
		{
			// invalid delegate, stop it
			bFinished = true;
			return;
		}

#pragma region check_pause_stop

		if (bStopped)
		{
			// stop now
			return Super::UpdateOperation(Response);
		}

		if (bPausedOneFrame)
		{
			bPausedOneFrame = false;
			return;
		}

		if (bPaused)
		{
			// skip processing
			return;
		}

#pragma endregion check_pause_stop
	}
}

FTimerHandle SetTimerForThisTick(UObject& WorldContextObject, const FTimerDelegate& InDelegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	auto& LatentActionManager = World->GetLatentActionManager();

	auto* TimerLatentAction = new FTimerLatentAction_Delay{};
	TimerLatentAction->Delegate = InDelegate;

	const auto Handle{FTimerHandle::NewHandle()};

	LatentActionManager.AddNewAction(&WorldContextObject, Handle, TimerLatentAction);

	return Handle;
}

FTimerHandle SetTimerForNextTick(UObject& WorldContextObject, const FTimerDelegate& InDelegate)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	auto& LatentActionManager = World->GetLatentActionManager();

	auto* TimerLatentAction = new FTimerLatentAction_Delay{};
	TimerLatentAction->Delegate = InDelegate;

	TimerLatentAction->bPausedOneFrame = 1;

	const auto Handle{FTimerHandle::NewHandle()};

	LatentActionManager.AddNewAction(&WorldContextObject, Handle, TimerLatentAction);

	return Handle;
}

template<typename T>
static FTimerHandle SetTimerHelper(UObject& WorldContextObject, const FTimerDelegate& InDelegate, const T& DelayParameter)
{
	if constexpr (std::is_same_v<T, FTimerParameterHelper_Time>)
	{
#if REM_WITH_DEVELOPMENT_ONLY_CODE
		if (!REM_ENSURE(DelayParameter.TimeToDelay >= 0.0f))
		{
			return {};
		}
#endif
	}

	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return {});

	auto& LatentActionManager = World->GetLatentActionManager();

	auto* TimerLatentAction = new FTimerLatentAction_Delay{InDelegate, DelayParameter};

	const auto Handle{FTimerHandle::NewHandle()};

	LatentActionManager.AddNewAction(&WorldContextObject, Handle, TimerLatentAction);

	return Handle;
}

FTimerHandle SetTimer(UObject& WorldContextObject, const FTimerDelegate& InDelegate,
	const FTimerParameterHelper_Time& DelayParameter)
{
	return SetTimerHelper(WorldContextObject, InDelegate, DelayParameter);
}

FTimerHandle SetTimer(UObject& WorldContextObject, const FTimerDelegate& InDelegate,
	const FTimerParameterHelper_Frame& DelayParameter)
{
	return SetTimerHelper(WorldContextObject, InDelegate, DelayParameter);
}

void PauseTimer(UObject& WorldContextObject, const FTimerHandle TimerHandle)
{
	SetTimerPaused(WorldContextObject, TimerHandle, true);
}

void UnpauseTimer(UObject& WorldContextObject, const FTimerHandle TimerHandle)
{
	SetTimerPaused(WorldContextObject, TimerHandle, false);
}

void SetTimerPaused(UObject& WorldContextObject, const FTimerHandle TimerHandle, const bool bPause)
{
	if (auto* TimerLatentAction = FindTimerAction(WorldContextObject, TimerHandle))
	{
		TimerLatentAction->bPaused = bPause;
	}
}

void SetTimerPausedOneFrame(UObject& WorldContextObject, const FTimerHandle TimerHandle, const bool bPause)
{
	if (auto* TimerLatentAction = FindTimerAction(WorldContextObject, TimerHandle))
	{
		TimerLatentAction->bPausedOneFrame = bPause;
	}
}

void StopTimer(UObject& WorldContextObject, FTimerHandle& TimerHandle)
{
	if (auto* TimerLatentAction = FindTimerAction(WorldContextObject, TimerHandle))
	{
		TimerLatentAction->bStopped = true;
	}

	TimerHandle = {};
}

FTimerLatentAction_Delay* FindTimerAction(UObject& WorldContextObject, const FTimerHandle TimerHandle)
{
	auto* World = WorldContextObject.GetWorld();
	RemCheckVariable(World, return nullptr);

	auto& LatentActionManager = World->GetLatentActionManager();

	return LatentActionManager.FindExistingAction<FTimerLatentAction_Delay>(&WorldContextObject, TimerHandle);
}

bool IsTimerActive(UObject& WorldContextObject, const FTimerHandle TimerHandle)
{
	return !!FindTimerAction(WorldContextObject, TimerHandle);
}

bool ResetTimerDelay(UObject& WorldContextObject, const FTimerHandle TimerHandle)
{
	if (auto* TimerLatentAction = FindTimerAction(WorldContextObject, TimerHandle))
	{
		if (TimerLatentAction->bTimeOrFrame)
		{
			TimerLatentAction->CurrentTimeOrFrame.Time = TimerLatentAction->TimeOrFrameToDelay.Time;
		}
		else
		{
			TimerLatentAction->CurrentTimeOrFrame.Frame = TimerLatentAction->TimeOrFrameToDelay.Frame;
		}
		return true;
	}

	return {};
}
}
