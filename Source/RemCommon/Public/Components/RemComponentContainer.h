// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemCppConcepts.h"
#include "Interface/RemScriptStructInterface.h"
#include "Macro/RemGenerateMembersMacro.h"

#include "RemComponentContainer.generated.h"

template<typename BaseStructT>
struct TInstancedStruct;

template<typename BaseStructT>
struct TConstStructView;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentBase
#if CPP
	: public IRemScriptStructInterface
#endif
{
	GENERATED_BODY()

    struct FContext
	{
	    UObject& OwnerRef;
	};
    
	virtual void Initialize(const FContext& Context);

	virtual bool ShouldTick(const FContext& Context) const;
	virtual void Tick(const FContext& Context, float DeltaSeconds);

	virtual void Uninitialize(const FContext& Context);
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentContainer
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FRemComponentBase>> Components;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UObject> Owner{};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bInitialized : 1{false};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bInitializing : 1{false};

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Component")
	uint8 bUnInitializing : 1{false};

public:
	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	auto FindComponent();
    
	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	auto FindComponent() const;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	auto FindComponent(int32 Index);
    
	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	auto FindComponent(int32 Index) const;

    template<std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	auto FindComponent(EnumClass Enum);

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	auto FindComponent(EnumClass Enum) const;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	void ForEachComponent(TFunctionRef<void(T&)> FunctionRef);

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	void ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const;

    
    /**
     * @return index of first component that matches the class exactly
     */
    template<std::derived_from<FRemComponentBase> TComponentType>
    int32 GetComponentIndex() const;
    
    /**
     * @return index of the component 
     */
    int32 GetComponentIndex(const FRemComponentBase& InComponent) const;
    
	void Initialize(UObject& OwnerRef);
	void Uninitialize();

	bool IsInitialized() const;

	void TryInitialize(UObject& OwnerRef);
	void TryUninitialize();

	void SetComponentsView(TConstArrayView<TConstStructView<FRemComponentBase>> InComponentsView);
	void SetComponentsView(TConstArrayView<TInstancedStruct<FRemComponentBase>> InComponentsView);
	void SetComponentsView(TArray<TInstancedStruct<FRemComponentBase>>&& InComponents);

    REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_SIMPLE(Components)
    
    FRemComponentContainer() = default;
    FRemComponentContainer(const FRemComponentContainer&) = delete;
    FRemComponentContainer(FRemComponentContainer&&) noexcept = delete;
    FRemComponentContainer& operator=(const FRemComponentContainer&) = delete;
    FRemComponentContainer& operator=(FRemComponentContainer&&) noexcept = delete;
    ~FRemComponentContainer() noexcept = default;
};

template<>
struct TStructOpsTypeTraits<FRemComponentContainer> : TStructOpsTypeTraitsBase2<FRemComponentContainer>
{
    enum
    {
        WithCopy = false
    };
};
