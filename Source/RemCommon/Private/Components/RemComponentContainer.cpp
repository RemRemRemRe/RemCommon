// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainer.inl"

#include "RemCommonLog.h"
#include "Macro/RemLogMacros.h"
#include "Misc/ScopeExit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentContainer)

void FRemComponentBase::Initialize(const FContext& Context)
{
}

bool FRemComponentBase::ShouldTick(const FContext& Context) const
{
	return false;
}

void FRemComponentBase::Tick(const FContext& Context, float)
{
}

void FRemComponentBase::Uninitialize(const FContext& Context)
{
}

void FRemComponentContainer::Initialize(UObject& OwnerRef)
{
	// make sure not calling this during UnInitializing
	RemCheckCondition(!bUnInitializing, return;);

	bInitializing = true;
	ON_SCOPE_EXIT
	{
		bInitializing = false;
	};

	Owner = &OwnerRef;

	// make sure Initialize can't be called by TryInitialize
	bInitialized = true;

	REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("initializing components, Owner:{0}"), OwnerRef);

	ForEachComponent<FRemComponentBase>(
		[&] (FRemComponentBase& Component)
	 {
		 Component.Initialize({OwnerRef});
	 });
}

void FRemComponentContainer::Uninitialize()
{
	// make sure not calling this during Initializing
	RemCheckCondition(!bInitializing, return;);

	bUnInitializing = true;
	ON_SCOPE_EXIT
	{
		bUnInitializing = false;
	};

	// make sure Uninitialize can't be called by TryUninitialize
	bInitialized = false;

	const auto CopyOwner = Owner;
	Owner = nullptr;

	REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("uninitializing components, Owner:{0}"), Owner);

	RemCheckVariable(CopyOwner, return;);

	ForEachComponent<FRemComponentBase>(
		[&] (FRemComponentBase& Component)
	 {
		 Component.Uninitialize({*CopyOwner});
	 });
}

bool FRemComponentContainer::IsInitialized() const
{
	return bInitialized;
}

void FRemComponentContainer::TryInitialize(UObject& OwnerRef)
{
	if (bInitialized)
	{
		REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("already initialized, Owner:{0}"), OwnerRef);
		return;
	}

	Initialize(OwnerRef);
}

void FRemComponentContainer::TryUninitialize()
{
	if (!bInitialized)
	{
		REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("it's not initialized, Owner:{0}"), Owner);
		return;
	}

	Uninitialize();
}

void FRemComponentContainer::CopyComponents(const TConstArrayView<TConstStructView<FRemComponentBase>> InComponentsView)
{
	TryUninitialize();

	Components = InComponentsView;
}

void FRemComponentContainer::CopyComponents(const TConstArrayView<TInstancedStruct<FRemComponentBase>> InComponentsView)
{
	TryUninitialize();

	Components = InComponentsView;
}

void FRemComponentContainer::MoveComponents(TArray<TInstancedStruct<FRemComponentBase>>&& InComponents)
{
	TryUninitialize();

	Components = std::move(InComponents);
}
