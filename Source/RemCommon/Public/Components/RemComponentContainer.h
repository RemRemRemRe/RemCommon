// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "RemCppConcepts.h"
#include "Interface/RemScriptStructInterface.h"
#include "Macro/RemGenerateMembersMacro.h"

#include "RemStructAsComponent.generated.h"

template<typename BaseStructT>
struct TInstancedStruct;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemStructAsComponentBase
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
struct REMCOMMON_API FRemStructAsComponents
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TArray<TInstancedStruct<FRemStructAsComponentBase>> Components;

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	T* GetComponent();

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	const T* GetComponent() const;

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	T* GetComponent(int32 Index);

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	const T* GetComponent(int32 Index) const;

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	T* GetComponent(EnumClass Enum);

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase, Rem::Concepts::is_scoped_enum EnumClass>
	const T* GetComponent(EnumClass Enum) const;

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	void ForEachComponent(TFunctionRef<void(T&)> FunctionRef);

	template<std::derived_from<FRemStructAsComponentBase> T = FRemStructAsComponentBase>
	void ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const;

	void Initialize(UObject& Owner);
	void Uninitialize(UObject& Owner);

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Components)
};
