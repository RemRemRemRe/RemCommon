// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Macro/RemGenerateMembersMacro.h"
#include "RemConcepts.h"
#include "RemCommonWrapperTypes.generated.h"

//////////////////////////////////////////////////////////////////////////
// Because container could not be nested at the moment, wrappers will be defined here.
//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemIntegerArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Integers;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftClassWrapper
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true))
	TSoftClassPtr<UObject> SoftClass;
	
	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_uobject<T>, SoftClass, , TSoftClassPtr<T>{SoftClass.ToSoftObjectPath()})

	using ThisType = FRemSoftClassWrapper;
	bool operator==(const ThisType&) const = default;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftClassArrayWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true, TitleProperty = SoftClass))
	TArray<FRemSoftClassWrapper> SoftClasses;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftObjectWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true))
	TSoftObjectPtr<UObject> SoftObject;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_uobject<T>, SoftObject, , TSoftObjectPtr<T>{SoftObject.ToSoftObjectPath()})

	using ThisType = FRemSoftObjectWrapper;
	bool operator==(const ThisType&) const = default;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftObjectArrayWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true, TitleProperty = SoftObject))
	TArray<FRemSoftObjectWrapper> SoftObjects;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemIntegerWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Number{};
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemFloatWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Number{};
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemDoubleWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Number{};
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftImageWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta=( DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface",
			DisallowedClasses = "/Script/MediaAssets.MediaTexture", AddFilterUI = true))
	TSoftObjectPtr<UObject> Image;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_image<T>, Image, , TSoftObjectPtr<T>{Image.ToSoftObjectPath()})
	
	using ThisType = FRemSoftImageWrapper;
	bool operator==(const ThisType&) const = default;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftMaterialWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true))
	TSoftObjectPtr<UMaterialInterface> Material;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_material_interface<T>, Material, , TSoftObjectPtr<T>{Material.ToSoftObjectPath()})

	using ThisType = FRemSoftMaterialWrapper;
	bool operator==(const ThisType&) const = default;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemSoftStreamableRenderAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true))
	TSoftObjectPtr<UStreamableRenderAsset> StreamableRenderAsset;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_streamable_render_asset<T>, StreamableRenderAsset, , TSoftObjectPtr<T>{StreamableRenderAsset.ToSoftObjectPath()})

	using ThisType = FRemSoftStreamableRenderAssetWrapper;
	bool operator==(const ThisType&) const = default;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemTextWrapper
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;
};

USTRUCT(BlueprintType)
struct REMCOMMON_API FRemDataAssetWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AddFilterUI = true))
	TWeakObjectPtr<const UDataAsset> DataAsset;

	REM_DEFINE_TEMPLATE_GETTER_RETURN_VALUE(requires Rem::Concepts::is_data_asset<T>, DataAsset, /*no suffix*/,
		Cast<T>(DataAsset.Get()))
};
