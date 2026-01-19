// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Interface/RemScriptStructInterface.h"
#include "RemCppConcepts.h"
#include "RemConcepts.h"
#include "Macro/RemGenerateMembersMacro.h"
#include "StructUtils/InstancedStructContainer.h"

#include "RemComponentViewContainer.generated.h"

struct FRemComponentViewContainerInstance;
template<typename BaseStructT>
struct TInstancedStruct;

template<typename BaseStructT>
struct TConstStructView;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentViewBase
#if CPP
	: public IRemScriptStructInterface
#endif
{
	GENERATED_BODY()

    struct FContext
	{
	    FRemComponentViewContainerInstance& OwnerInstance;
	    const int32 ComponentIndex;
	};
    
	virtual FConstStructView GetInstanceDataType(const FContext& Context) const;
    
	virtual void Initialize(const FContext& Context) const;

	virtual bool ShouldTick(const FContext& Context) const;
	virtual void Tick(const FContext& Context, float DeltaSeconds) const;

	virtual void Uninitialize(const FContext& Context) const;
    
    template<std::derived_from<FRemComponentViewBase> TComponentType>
    TComponentType::FInstanceDataType* GetInstanceData(const FContext& Context) const;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentViewContainer
{
    GENERATED_BODY()
    
protected:
#if WITH_EDITORONLY_DATA
    
    UPROPERTY(EditAnywhere, Category = "Component", meta = (ExcludeBaseStruct))
    TArray<TInstancedStruct<FRemComponentViewBase>> Components;
    
#endif
    
    UPROPERTY(EditAnywhere, Category = "Rem")
    FInstancedStructContainer ComponentContainer{};
    
public:
    
#if WITH_EDITORONLY_DATA
    
    /**
     * it has to be called somewhere in the editor code to make ComponentContainer valid
     */
    void SyncData();
    
#endif
    
    REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE(Components, ComponentContainer)
};

/**
 * readonly component + instance data
 */
USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentViewContainerInstance
{
	GENERATED_BODY()

protected:
	TArray<TConstStructView<FRemComponentViewBase>> Components;
    
    UPROPERTY(EditAnywhere, Category = "Rem")
    FInstancedStructContainer InstanceDataContainer{};
    
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UObject> Owner{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bInitialized : 1{false};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bInitializing : 1{false};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bUnInitializing : 1{false};

public:
    using FElementType = TConstStructView<FRemComponentViewBase>;

	template<std::derived_from<FRemComponentViewBase> T = FRemComponentViewBase>
	auto FindComponent() const;

	template<std::derived_from<FRemComponentViewBase> T = FRemComponentViewBase>
	auto FindComponent(int32 Index) const;

	template<std::derived_from<FRemComponentViewBase> T = FRemComponentViewBase, Rem::Concepts::is_scoped_enum EnumClass>
	auto FindComponent(EnumClass Enum) const;

	template<std::derived_from<FRemComponentViewBase> T = FRemComponentViewBase>
	void ForEachComponent(TFunctionRef<void(const T& Struct, int32 Index, const UScriptStruct& ScriptStruct)> FunctionRef) const;
    
    FStructView GetInstanceDataView(int32 ComponentIndex);
    
    template<std::derived_from<FRemComponentViewBase> TComponentType>
    TStructView<typename TComponentType::FInstanceDataType> GetInstanceData(int32 ComponentIndex);
    
    template<std::derived_from<FRemComponentViewBase> TComponentType>
    TStructView<typename TComponentType::FInstanceDataType> GetInstanceData();
    
    /**
     * @return index of first component that matches the class exactly
     */
    template<std::derived_from<FRemComponentViewBase> TComponentType>
    int32 GetComponentIndex() const;
    
    /**
     * @return index of the component 
     */
    int32 GetComponentIndex(const FRemComponentViewBase& InComponent) const;
    
	void Initialize(UObject& OwnerRef);
	void Uninitialize();

	bool IsInitialized() const;

	void TryInitialize(UObject& OwnerRef);
	void TryUninitialize();

	void SetComponentsView(TConstArrayView<TConstStructView<FRemComponentViewBase>> InComponentsView);
	void SetComponentsView(TConstArrayView<TInstancedStruct<FRemComponentViewBase>> InComponentsView);
	void SetComponentsView(const FInstancedStructContainer& InStructContainer);

    template<Rem::Concepts::is_uobject TOwner>
    TOwner* GetOwner() const;
    
    FRemComponentViewContainerInstance() = default;
    FRemComponentViewContainerInstance(const FRemComponentViewContainerInstance&) = delete;
    FRemComponentViewContainerInstance(FRemComponentViewContainerInstance&&) noexcept = delete;
    FRemComponentViewContainerInstance& operator=(const FRemComponentViewContainerInstance&) = delete;
    FRemComponentViewContainerInstance& operator=(FRemComponentViewContainerInstance&&) noexcept = delete;
    ~FRemComponentViewContainerInstance() noexcept = default;
};

template<>
struct TStructOpsTypeTraits<FRemComponentViewContainerInstance> : TStructOpsTypeTraitsBase2<FRemComponentViewContainerInstance>
{
    enum
    {
        WithCopy = false
    };
};
