// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "Macro/RemGenerateMembersMacro.h"

#include "RemCommonWrapperTypes.generated.h"

class UCurveBase;

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemByteWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	uint8 Byte{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Byte)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemByteArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TArray<uint8> Bytes{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Bytes)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemIntegerWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	int32 Number{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Number)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemIntegerArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
    TArray<int32> Integers{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Integers)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemFloatWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	float Number{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Number)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemFloatArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TArray<float> Numbers{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Numbers)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemDoubleWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	double Number{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Number)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemDoubleArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TArray<double> Numbers{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Numbers)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTextWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	FText Text{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Text)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTextArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem")
	TArray<FText> Texts{};

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Texts)
};

USTRUCT(Blueprintable)
struct FRemCurveBaseWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TObjectPtr<const UCurveBase> Curve{};

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UCurveBase>, Curve, /*no suffix*/, Cast<T>(Curve.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Curve.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UCurveBase>, Object, /*no suffix*/, Cast<T>(Curve.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Curve.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemDataAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TObjectPtr<const UDataAsset> DataAsset{};

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, DataAsset, /*no suffix*/, Cast<T>(DataAsset.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, DataAsset.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, Object, /*no suffix*/, Cast<T>(DataAsset.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, DataAsset.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakDataAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TWeakObjectPtr<const UDataAsset> DataAsset;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, DataAsset, /*no suffix*/, Cast<T>(DataAsset.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, Object, /*no suffix*/, Cast<T>(DataAsset.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, DataAsset)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftDataAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSoftObjectPtr<const UDataAsset> DataAsset;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, DataAsset, /*no suffix*/, Cast<T>(DataAsset.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UDataAsset>, Object, /*no suffix*/, Cast<T>(DataAsset.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, DataAsset)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemObjectWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TObjectPtr<UObject> Object{};

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Object.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, Cast<T>(Object.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Object.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemObjectArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Object))
	TArray<FRemObjectWrapper> Objects;

	using ThisType = FRemObjectArrayWrapper;
    using IsArrayOfObjectWrapper = std::true_type;
    using WrapperElementType = decltype(Objects)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Objects)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Objects)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakObjectWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TWeakObjectPtr<UObject> Object;

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, Cast<T>(Object.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Object)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakObjectArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Object))
	TArray<FRemWeakObjectWrapper> Objects;

	using ThisType = FRemWeakObjectArrayWrapper;
	using IsArrayOfObjectWrapper = std::true_type;
	using WrapperElementType = decltype(Objects)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Objects)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Objects)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemConstObjectWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TObjectPtr<const UObject> Object{};

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Object.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, Cast<T>(Object.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Object.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemConstObjectArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Object))
	TArray<FRemConstObjectWrapper> Objects;

	using ThisType = FRemConstObjectArrayWrapper;
    using IsArrayOfObjectWrapper = std::true_type;
    using WrapperElementType = decltype(Objects)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Objects)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Objects)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemConstWeakObjectWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TWeakObjectPtr<const UObject> Object;

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, Cast<T>(Object.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Object)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemConstWeakObjectArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Object))
	TArray<FRemConstWeakObjectWrapper> Objects;

	using ThisType = FRemConstWeakObjectArrayWrapper;
	using IsArrayOfObjectWrapper = std::true_type;
	using WrapperElementType = decltype(Objects)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Objects)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Objects)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftObjectWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSoftObjectPtr<const UObject> Object;

	using ThisType = FRemSoftObjectWrapper;
	bool operator==(const ThisType&) const = default;

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, Cast<T>(Object.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Object)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftObjectArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Object))
	TArray<FRemSoftObjectWrapper> Objects;

	using ThisType = FRemSoftObjectArrayWrapper;
	using IsArrayOfObjectWrapper = std::true_type;
	using WrapperElementType = decltype(Objects)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Objects)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Objects)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftClassWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSoftClassPtr<UObject> Class;

	using ThisType = FRemSoftClassWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_GETTERS_RETURN_REFERENCE_SIMPLE(Class)

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Class.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Object, /*no suffix*/, TSubclassOf<T>{Class.Get()})
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UObject>, Class, /*no suffix*/, GetObject<T>())

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Class)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftClassArrayWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true, TitleProperty = Class))
	TArray<FRemSoftClassWrapper> Classes;

	using ThisType = FRemSoftClassArrayWrapper;
	using IsArrayOfObjectWrapper = std::true_type;
	using WrapperElementType = decltype(Classes)::ElementType;

	REM_DEFINE_GETTERS_RETURN_REFERENCE(Elements, /*no suffix*/, Classes)

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Classes)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemImageWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem",
		meta=( DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture", AddFilterUI = true))
	TObjectPtr<const UObject> Image{};

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Image, /*no suffix*/, Cast<T>(Image.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Image.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Object, /*no suffix*/, Cast<T>(Image.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Image.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakImageWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem",
		meta=( DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture", AddFilterUI = true))
	TWeakObjectPtr<const UObject> Image;

	using ThisType = FRemWeakImageWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Image, /*no suffix*/, Cast<T>(Image.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Image)
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Object, /*no suffix*/, Cast<T>(Image.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Image)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftImageWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem",
		meta=( DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture", AddFilterUI = true))
	TSoftObjectPtr<const UObject> Image;

	using ThisType = FRemSoftImageWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Image, /*no suffix*/, Cast<T>(Image.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Image)
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(Rem::Concepts::is_image, Object, /*no suffix*/, Cast<T>(Image.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Image)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemMaterialWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TObjectPtr<UMaterialInterface> Material{};

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Material, /*no suffix*/, Cast<T>(Material.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Material.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Object, /*no suffix*/, Cast<T>(Material.Get()))

	REM_DEFINE_GETTERS_RETURN_VALUE(/*no predicate*/, /*no suffix*/, Material.Get())
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakMaterialWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TWeakObjectPtr<UMaterialInterface> Material;

	using ThisType = FRemWeakMaterialWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Material, /*no suffix*/, Cast<T>(Material.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, Material.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Object, /*no suffix*/, Cast<T>(Material.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Material)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftMaterialWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSoftObjectPtr<const UMaterialInterface> Material;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Material, /*no suffix*/, Cast<T>(Material.Get()))

	using ThisType = FRemSoftMaterialWrapper;
	bool operator==(const ThisType&) const = default;

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UMaterialInterface>, Object, /*no suffix*/, Cast<T>(Material.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, Material)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemStreamableRenderAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	const UStreamableRenderAsset* StreamableRenderAsset{};

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, StreamableRenderAsset, /*no suffix*/, Cast<T>(StreamableRenderAsset))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, StreamableRenderAsset)
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, Object, /*no suffix*/, Cast<T>(StreamableRenderAsset))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, StreamableRenderAsset)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemWeakStreamableRenderAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TWeakObjectPtr<const UStreamableRenderAsset> StreamableRenderAsset;

	using ThisType = FRemWeakStreamableRenderAssetWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, StreamableRenderAsset, , Cast<T>(StreamableRenderAsset.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_GETTERS_RETURN_VALUE(Object, /*no suffix*/, StreamableRenderAsset.Get())
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, Object, /*no suffix*/, Cast<T>(StreamableRenderAsset.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, StreamableRenderAsset)
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftStreamableRenderAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rem", meta = (AddFilterUI = true))
	TSoftObjectPtr<const UStreamableRenderAsset> StreamableRenderAsset;

	using ThisType = FRemSoftStreamableRenderAssetWrapper;
	bool operator==(const ThisType&) const = default;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, StreamableRenderAsset, , Cast<T>(StreamableRenderAsset.Get()))

	using IsObjectWrapper = std::true_type;
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(std::derived_from<UStreamableRenderAsset>, Object, /*no suffix*/, Cast<T>(StreamableRenderAsset.Get()))

	REM_DEFINE_GETTERS_RETURN_REFERENCE(/*no predicate*/, /*no suffix*/, StreamableRenderAsset)
};
