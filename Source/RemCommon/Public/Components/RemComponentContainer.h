// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "RemCppConcepts.h"
#include "Interface/RemScriptStructInterface.h"
#include "Macro/RemGenerateMembersMacro.h"

#include "RemComponentContainer.generated.h"

template<typename BaseStructT>
struct TInstancedStruct;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentBase
#if CPP
	: public IRemScriptStructInterface
#endif
{
	GENERATED_BODY()

	virtual void Initialize(UObject& Owner);

	virtual bool ShouldTick(UObject& Owner) const;
	virtual void Tick(UObject& Owner, float DeltaSeconds);

	virtual void Uninitialize(UObject& Owner);
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemComponentContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TArray<TInstancedStruct<FRemComponentBase>> Components;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	T* GetComponent();

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	const T* GetComponent() const;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	T* GetComponent(int32 Index);

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	const T* GetComponent(int32 Index) const;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	T* GetComponent(EnumClass Enum);

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	const T* GetComponent(EnumClass Enum) const;

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	void ForEachComponent(TFunctionRef<void(T&)> FunctionRef);

	template<std::derived_from<FRemComponentBase> T = FRemComponentBase>
	void ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const;

	void Initialize(UObject& Owner);
	void Uninitialize(UObject& Owner);

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Components)
};
