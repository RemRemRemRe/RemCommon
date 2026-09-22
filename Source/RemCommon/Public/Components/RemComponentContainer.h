// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "Interface/RemScriptStructInterface.h"

#include "RemCppConcepts.h"
#include "RemConcepts.h"
#include "Macro/RemGenerateMembersMacro.h"
#include "StructUtils/InstancedStructContainer.h"
#include "RemNotNull.h"

#include "RemComponentContainer.generated.h"

struct FRemComponentContainer;
template <typename BaseStructT>
struct TInstancedStruct;

template <typename BaseStructT>
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
        Rem::TNotNull<FRemComponentContainer*> OwnerInstance;
        const int32 ComponentIndex;
    };

    virtual void Initialize(const FContext& Context);

    virtual bool ShouldTick(const FContext& Context) const;
    virtual void Tick(const FContext& Context, float DeltaSeconds);

    virtual void Uninitialize(const FContext& Context);

    template <Rem::CUObject TOwner>
    [[nodiscard]] static TOwner* GetOwner(const FContext& Context);

    template <Rem::CAActor TOwnerActor = AActor>
    [[nodiscard]] static TOwnerActor* GetOwnerOfOwner(const FContext& Context);
};

/**
 * every component is writable
 */
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
    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    [[nodiscard]] auto FindComponent();

    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    [[nodiscard]] auto FindComponent() const;

    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    [[nodiscard]] auto FindComponent(int32 Index);

    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    [[nodiscard]] auto FindComponent(int32 Index) const;

    template <std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::CScopedEnum EnumClass>
    [[nodiscard]] auto FindComponent(EnumClass Enum);

    template <std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::CScopedEnum EnumClass>
    [[nodiscard]] auto FindComponent(EnumClass Enum) const;

    /**
     * @brief Iterates the container's T components.
     * @param FunctionRef receives element and script-struct pointers that are never null; the element pointer aliases the container's stored element.
     */
    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    void ForEachComponent(
        TFunctionRef<void(Rem::TNotNull<T*>, int32, Rem::TNotNull<const UScriptStruct*>)> FunctionRef);

    /**
     * @brief Iterates the container's T components.
     * @param FunctionRef receives element and script-struct pointers that are never null; the element pointer aliases the container's stored element.
     */
    template <std::derived_from<FRemComponentBase> T = FRemComponentBase>
    void ForEachComponent(
        TFunctionRef<void(Rem::TNotNull<const T*>, int32, Rem::TNotNull<const UScriptStruct*>)> FunctionRef) const;

    /**
     * @return index of first component that matches the class exactly
     */
    template <std::derived_from<FRemComponentBase> TComponentType>
    int32 GetComponentIndex() const;

    /**
     * @return index of the component, or INDEX_NONE if the instance is not stored in this container
     */
    [[nodiscard]] int32 GetComponentIndex(Rem::TNotNull<const FRemComponentBase*> InComponent) const;

    void Initialize(Rem::TNotNull<UObject*> OwnerRef);
    void Uninitialize();

    bool IsInitialized() const;

    void TryInitialize(Rem::TNotNull<UObject*> OwnerRef);
    void TryUninitialize();

    void SetComponentsView(TConstArrayView<TConstStructView<FRemComponentBase>> InComponentsView);
    void SetComponentsView(TConstArrayView<TInstancedStruct<FRemComponentBase>> InComponentsView);
    void SetComponentsView(TArray<TInstancedStruct<FRemComponentBase>>&& InComponents);

    template <Rem::CUObject TOwner>
    TOwner* GetOwner() const;

    REM_DEFINE_CONST_ONLY_GETTERS_RETURN_REFERENCE_SIMPLE(Components)

    FRemComponentContainer()                                             = default;
    FRemComponentContainer(const FRemComponentContainer&)                = delete;
    FRemComponentContainer(FRemComponentContainer&&) noexcept            = delete;
    FRemComponentContainer& operator=(const FRemComponentContainer&)     = delete;
    FRemComponentContainer& operator=(FRemComponentContainer&&) noexcept = delete;
    ~FRemComponentContainer() noexcept                                   = default;
};

template <>
struct TStructOpsTypeTraits<FRemComponentContainer> : TStructOpsTypeTraitsBase2<FRemComponentContainer>
{
    enum
    {
        WithCopy = false
    };
};
