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

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	T* GetComponent();

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	const T* GetComponent() const;

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	T* GetComponent(int32 Index);

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	const T* GetComponent(int32 Index) const;

	template<typename T = FRemStructAsComponentBase, typename EnumClass>
	requires std::is_base_of_v<FRemStructAsComponentBase, T> && Rem::Concepts::is_scoped_enum<EnumClass>
	T* GetComponent(EnumClass Enum);

	template<typename T = FRemStructAsComponentBase, typename EnumClass>
	requires std::is_base_of_v<FRemStructAsComponentBase, T> && Rem::Concepts::is_scoped_enum<EnumClass>
	const T* GetComponent(EnumClass Enum) const;

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	void ForEachComponent(TFunctionRef<void(T&)> FunctionRef);

	template<typename T = FRemStructAsComponentBase>
	requires std::is_base_of_v<FRemStructAsComponentBase, T>
	void ForEachComponent(TFunctionRef<void(const T&)> FunctionRef) const;

	void Initialize(UObject& Owner);
	void Uninitialize(UObject& Owner);

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Components)
};
