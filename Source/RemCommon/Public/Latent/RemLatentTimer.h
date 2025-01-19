// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "LatentActions.h"

namespace Rem::Latent
{
	using FTimerDelegate = TDelegate<void(), FNotThreadSafeNotCheckedDelegateUserPolicy>;

	/**
	 * 0 means invalid handle (default to 0)
	 */
	struct REMCOMMON_API FTimerHandle
	{
		uint32 Handle{0};

		using FHandleType = decltype(Handle);

		bool IsValid() const { return Handle != 0; }

		operator FHandleType() const { return Handle; }

		FHandleType& operator*() { return Handle; }
		const FHandleType& operator*() const { return Handle; }

		static FTimerHandle NewHandle();
	};

	/**
	 * by default, it set to execute this frame, no loop, no initial delay, up to call once per frame
	 *
	 * @note do set bSkipCountingThisFrame to TRUE if you're set timer WITHIN a latent timer callback
	 * or it may cause infinite loop when delta time is larger enough than your TimeToDelay
	 *
	 * @LoopCount 0 means loop forever
	 */
	struct REMCOMMON_API FTimerParameterHelper_Time
	{
		float TimeToDelay{0.0f};
		uint32 LoopCount{1};
		float InitialDelay{-1.0f};
		bool bMaxOncePerFrame{false};
		bool bSkipCountingThisFrame{false};
	};

	/**
	 * by default, it set to execute this frame, no loop, no initial delay, up to call once per frame
	 *
	 * @LoopCount 0 means loop forever
	 *
	 * @note the frame format could only be called up to once per frame
	 */
	struct REMCOMMON_API FTimerParameterHelper_Frame
	{
		uint32 FrameToDelay{0};
		uint32 LoopCount{1};
		uint32 InitialDelay{std::numeric_limits<uint32>::max()};
		bool bSkipCountingThisFrame{false};
	};

	class REMCOMMON_API FTimerLatentActionBase : public FPendingLatentAction
	{
		using Super = FPendingLatentAction;

	public:
		FTimerDelegate Delegate;
	};

	/**
	 * by default, it set to execute this frame, no loop, no initial delay, up to call once per frame
	 *
	 * we are counting from this frame, from index 0
	 *
	 * this frame, aka 0 frame(frame at index 0), or the first frame
	 *
	 * if you want to call back at the third frame(frame at index 2), you could pass FTimerParameterHelper_Frame
	 * whose FrameToDelay is 2 or InitialDelay is 2 with other value to be defaulted
	 *
	 * when bSkipCountingThisFrame is true, you will start counting from "second frame(frame at index 1)" in the context above
	 * in this case, with FrameToDelay set to 2, the call back will fire at "fourth frame(frame at index 3)" in the context above
	 *
	 * @note LevelCollection related functionality is not supported as the engine does, you have to do it on your own delegates
	 *	@see FTimerData::LevelCollection, FScopedLevelCollectionContextSwitch
	 */
	class REMCOMMON_API FTimerLatentAction_Delay : public FTimerLatentActionBase
	{
		using Super = FTimerLatentActionBase;

	public:
		/**
		 * 4 bytes for timer counter
		 */
		union FTimeOrFrame32
		{
			uint32 Frame{0};
			float Time;
		} TimeOrFrameToDelay{};

		/**
		 * CurrentTimeOrFrame in frame format has to start from 1 rather than 0,
		 * because it will be "--" first in UpdateOperation
		 */
		FTimeOrFrame32 CurrentTimeOrFrame{1};

		/**
		 * "format" of TimeOrFrameToDelay, default to frame. 1 is time, 0 is frame
		 */
		uint32 bTimeOrFrame : 1{0};

		uint32 bPaused : 1{0};
		uint32 bPausedOneFrame : 1{0};
		uint32 bStopped : 1{0};

		/**
		 * @see FTimerData::bMaxOncePerFrame
		 */
		uint32 bMaxOncePerFrame : 1{0};


		//// bits wasted ////
		//uint32 Wasted : 27{0};

		/**
		 * 4 bytes for loop count
		 *
		 * 0 means loop forever, 1 means no loop,
		 */
		uint32 LoopCount{1};

		FTimerLatentAction_Delay() = default;
		FTimerLatentAction_Delay(const FTimerDelegate& InDelegate, const FTimerParameterHelper_Time& DelayParameter);
		FTimerLatentAction_Delay(const FTimerDelegate& InDelegate, const FTimerParameterHelper_Frame& DelayParameter);

	protected:
		virtual void UpdateOperation(FLatentResponse& Response) override;
	};

	/**
	 * @note if you are writing something like "retry every frame" or "retry next frame", use SetTimerForNextTick
	 * or infinite loop happens if this called within FLatentActionManager::ProcessLatentActions
	 *
	 * and it will call back in next frame if FLatentActionManager::ProcessLatentActions is called on WorldContextObject this frame
	 *
	 * consider using FTimerManager if you do need do it this frame (before the end of this frame)
	 */
	REMCOMMON_API FTimerHandle SetTimerForThisTick(UObject& WorldContextObject, const FTimerDelegate& InDelegate);

	/**
	 * This is preferred and safer to call than the one above
	 */
	REMCOMMON_API FTimerHandle SetTimerForNextTick(UObject& WorldContextObject, const FTimerDelegate& InDelegate);

	REMCOMMON_API FTimerHandle SetTimer(UObject& WorldContextObject, const FTimerDelegate& InDelegate, const FTimerParameterHelper_Time& DelayParameter);
	REMCOMMON_API FTimerHandle SetTimer(UObject& WorldContextObject, const FTimerDelegate& InDelegate, const FTimerParameterHelper_Frame& DelayParameter);

	REMCOMMON_API void PauseTimer(UObject& WorldContextObject, FTimerHandle TimerHandle);
	REMCOMMON_API void UnpauseTimer(UObject& WorldContextObject, FTimerHandle TimerHandle);
	REMCOMMON_API void SetTimerPaused(UObject& WorldContextObject, FTimerHandle TimerHandle, bool bPause);

	REMCOMMON_API void SetTimerPausedOneFrame(UObject& WorldContextObject, FTimerHandle TimerHandle, bool bPause);

	REMCOMMON_API void StopTimer(UObject& WorldContextObject, FTimerHandle TimerHandle);

	REMCOMMON_API FTimerLatentAction_Delay* FindTimerAction(UObject& WorldContextObject, FTimerHandle TimerHandle);
	REMCOMMON_API bool IsTimerActive(UObject& WorldContextObject, FTimerHandle TimerHandle);

	/**
	 * this could mimic re triggerable timer, won't affect loop count though
	 *
	 * @return true if found the timer and reset it's delay, time or frame
	 */
	REMCOMMON_API bool ResetTimerDelay(UObject& WorldContextObject, FTimerHandle TimerHandle);
}
