// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "Macro/RemGenerateMembersMacro.h"
#include "Templates/SubclassOf.h"

#include "RemClassWrapper.generated.h"

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemClassWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSubclassOf<UObject> Class{};

	using ThisType = FRemClassWrapper;
	bool operator==(const ThisType&) const = default;

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Class.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_uobject<T>, Object, /*no suffix*/, TSubclassOf<T>{Class}.Get())

	REM_DEFINE_GETTERS_RETURN_VALUE(Class, /* no suffix */, Class.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_uobject<T>, Class, /*no suffix*/, TSubclassOf<T>{Class}.Get())

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_uobject<T>, ClassT, /*no suffix*/, TSubclassOf<T>{Class})

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Class.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemClassArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Class))
	TArray<FRemClassWrapper> Classes;

	using ThisType = FRemClassArrayWrapper;
	using IsArrayOfObjectWrapper = std::true_type;
	using WrapperElementType = decltype(Classes)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Classes)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Classes)
};
