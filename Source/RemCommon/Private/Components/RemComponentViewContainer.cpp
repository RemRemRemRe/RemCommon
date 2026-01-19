// Copyright RemRemRemRe. 2025. All Rights Reserved.


#include "Components/RemComponentViewContainer.h"
#include "Components/RemComponentViewContainer.inl"

#include "RemCommonLog.h"
#include "Macro/RemLogMacros.h"
#include "Misc/ScopeExit.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentViewContainer)

FConstStructView FRemComponentViewBase::GetInstanceDataType(const FContext& Context) const
{
    return {};
}

void FRemComponentViewBase::Initialize(const FContext& Context) const
{
}

bool FRemComponentViewBase::ShouldTick(const FContext& Context) const
{
	return false;
}

void FRemComponentViewBase::Tick(const FContext& Context, float) const
{
}

void FRemComponentViewBase::Uninitialize(const FContext& Context) const
{
}

#if WITH_EDITORONLY_DATA

void FRemComponentViewContainer::SyncData()
{
    ComponentContainer.Reset();
    
    TArray<FConstStructView> NewViews;
    NewViews.Reserve(Components.Num());
    Algo::Transform(Components, NewViews, [](const TInstancedStruct<FRemComponentViewBase>& Struct)
    {
        return FConstStructView{Struct.GetScriptStruct(), Struct.GetMemory()};
    });
    
    ComponentContainer.Append(NewViews);
}

#endif

FStructView FRemComponentViewContainerInstance::GetInstanceDataView(const int32 ComponentIndex)
{
    return InstanceDataContainer[ComponentIndex];
}

int32 FRemComponentViewContainerInstance::GetComponentIndex(const FRemComponentViewBase& InComponent) const
{
    for (int32 Index = 0; Index < Components.Num(); ++Index)
    {
        if (reinterpret_cast<const uint8*>(&InComponent) == Components[Index].GetMemory())
        {
            return Index;
        }
    }
    
    return INDEX_NONE;
}

void FRemComponentViewContainerInstance::Initialize(UObject& OwnerRef)
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

	{
        TArray<FConstStructView> InstanceDataTypes;
	    InstanceDataTypes.Reserve(Components.Num());
        ForEachComponent<FRemComponentViewBase>(
        [&] (const FRemComponentViewBase& Component, const int32 Index, const UScriptStruct&)
         {
             InstanceDataTypes.Add(Component.GetInstanceDataType({*this, Index}));
         });
	    
#if REM_WITH_DEVELOPMENT_ONLY_CODE
	    
	    RemCheckCondition(InstanceDataContainer.Num() == 0);
	    
#endif
	    
	    InstanceDataContainer.InsertAt(0, InstanceDataTypes);
	}
    
	ForEachComponent<FRemComponentViewBase>(
		[&] (const FRemComponentViewBase& Component, const int32 Index, const UScriptStruct&)
	 {
		 Component.Initialize({*this, Index});
	 });
}

void FRemComponentViewContainerInstance::Uninitialize()
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

	ForEachComponent<FRemComponentViewBase>(
		[&] (const FRemComponentViewBase& Component, const int32 Index, const UScriptStruct&)
	 {
		 Component.Uninitialize({*this, Index});
	 });
    
    InstanceDataContainer.Reset();
}

bool FRemComponentViewContainerInstance::IsInitialized() const
{
	return bInitialized;
}

void FRemComponentViewContainerInstance::TryInitialize(UObject& OwnerRef)
{
	if (bInitialized)
	{
		REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("already initialized, Owner:{0}"), OwnerRef);
		return;
	}

	Initialize(OwnerRef);
}

void FRemComponentViewContainerInstance::TryUninitialize()
{
	if (!bInitialized)
	{
		REM_LOG_FUNCTION(LogRemCommon, Verbose, TEXT("it's not initialized, Owner:{0}"), Owner);
		return;
	}

	Uninitialize();
}

void FRemComponentViewContainerInstance::SetComponentsView(const TConstArrayView<TConstStructView<FRemComponentViewBase>> InComponentsView)
{
	TryUninitialize();

	Components = InComponentsView;
}

void FRemComponentViewContainerInstance::SetComponentsView(const TConstArrayView<TInstancedStruct<FRemComponentViewBase>> InComponentsView)
{
	TryUninitialize();

	Components = InComponentsView;
}

void FRemComponentViewContainerInstance::SetComponentsView(const FInstancedStructContainer& InStructContainer)
{
    TryUninitialize();
    
    Components.Reset(InStructContainer.Num());

    for (const auto& View : InStructContainer)
    {
        RemCheckVariable(View.GetPtr<const FRemComponentViewBase>(), continue;);
        
        Components.AddDefaulted_GetRef().SetStructData(View.GetScriptStruct(), View.GetMemory());
    }
}
