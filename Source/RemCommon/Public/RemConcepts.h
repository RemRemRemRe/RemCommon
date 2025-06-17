// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Engine/EngineBaseTypes.h"

class UGameInstance;
class UCurveLinearColor;
class UCurveVector;
class UCurveFloat;
class UCurveBase;
class AHUD;
class UCameraModifier;
class UPoseAsset;
class UAnimStreamable;
class UAnimComposite;
class UAimOffsetBlendSpace;
class UBlendSpace;
class UAnimMontage;
class UAnimSequence;
class UAnimSequenceBase;
class UAnimationAsset;
class UMediaTexture;
struct FGameplayTag;
class FSoftObjectProperty;
class FObjectPropertyBase;
class FProperty;
class UMaterialInterface;
class UDataAsset;
class UPrimaryDataAsset;
class APlayerCameraManager;
class APlayerController;
class AController;
class APawn;
class APlayerState;
class UActorComponent;
class AActor;
class UObject;
class UWorld;
struct FSoftObjectPath;
class UClass;
class UScriptStruct;
enum ENetRole : int;
//enum ENetMode;
class FString;
class AAIController;
class ISlateTextureAtlasInterface;
class ULocalPlayer;
class AGameStateBase;
class AGameModeBase;
class UAnimInstance;
class UStreamableRenderAsset;
struct FTableRowBase;
template <typename T>
struct TIsUEnumClass;
class FDelegateHandle;

namespace Rem::Concepts
{
	template<class T>
	concept has_get_world = requires (UWorld* World, const T Object)
	{
		World = Object.GetWorld();
	};

	template<class T>
	concept has_get_local_role = requires (ENetRole Role, const T Object)
	{
		Role = Object.GetLocalRole();
	};

	template<class T>
	concept has_get_owner_role = requires (ENetRole Role, const T Object)
	{
		Role = Object.GetOwnerRole();
	};

	template<class T>
	concept has_get_net_mode = requires (ENetMode Mode, const T Object)
	{
		Mode = Object.GetNetMode();
	};

	template<class T>
	concept has_get_owner = requires (const T Object)
	{
		Object.GetOwner();
	};

	template<class T>
	concept has_get = requires (const T Object)
	{
		Object.Get();
	};

	template<class T>
	concept has_tag = requires (FGameplayTag& Tag, const T Object)
	{
		Tag = T::Tag;
	};

	template<class T>
	concept has_get_tag = requires (FGameplayTag& Tag, const T Object)
	{
		Tag = Object.GetTag();
	};

	template<class T>
	concept has_is_net_mode = requires (ENetMode Mode, const T Object, bool Result)
	{
		Result = Object.IsNetMode(Mode);
	};

	template<class T>
	concept has_to_compact_string = requires (FString& String, const T Object)
	{
		String = Object.ToCompactString();
	};

	template<class T>
	concept has_to_string_simple = requires (FString& String, const T Object)
	{
		String = Object.ToStringSimple();
	};

	template<class T>
	concept has_to_simple_string = requires (FString& String, const T Object)
	{
		String = Object.ToSimpleString();
	};

	template<class T>
	concept has_to_string = requires (FString& String, const T Object)
	{
		String = Object.ToString();
	};

	template<class T>
	concept has_lex_to_string = requires (FString& String, const T Object)
	{
		String = LexToString(Object);
	};

	template<class T>
	concept has_static_struct = requires (UScriptStruct*& StructRef)
	{
		StructRef = T::StaticStruct();
	};

	template<class T>
	concept has_static_class = requires (UClass*& StructRef)
	{
		StructRef = T::StaticClass();
	};

	template<class T>
	concept has_get_name = requires (FString& String, const T Object)
	{
		String = Object.GetName();
	};

	template<class T>
	concept has_get_full_name = requires (FString& String, const T Object)
	{
		String = Object.GetFullName();
	};

	template<class T>
	concept is_stringable =
		std::is_same_v<bool, std::remove_cvref_t<T>>
		|| has_to_compact_string<T>
		|| has_to_string_simple<T>
		|| has_to_simple_string<T>
		|| has_to_string<T>
		|| has_lex_to_string<T>
		|| has_get_name<T>
		|| std::is_same_v<FDelegateHandle, std::remove_cvref_t<T>>
		|| TIsUEnumClass<T>::Value
		|| has_static_struct<T>
	;

	template<class T>
	concept has_is_valid = requires (bool Result, const T Object)
	{
		Result = Object.IsValid();
	};

	template<class T>
	concept has_to_soft_object_path = requires (FSoftObjectPath& SoftPath, bool bNull, const T Object)
	{
		SoftPath = Object.ToSoftObjectPath();
	};

	template<class T>
	concept is_world = std::derived_from<T, UWorld>;

	template<class T>
	concept is_game_instance = std::derived_from<T, UGameInstance>;

	template<class T>
	concept is_uobject = std::derived_from<T, UObject>;

	template<class T>
	concept is_uclass = std::derived_from<T, UClass>;

	template<class T>
	concept is_hud = std::derived_from<T, AHUD>;

	template<class T>
	concept is_actor = std::derived_from<T, AActor>;

	template<class T>
	concept is_actor_component = std::derived_from<T, UActorComponent>;

	template<class T>
	concept is_player_state = std::derived_from<T, APlayerState>;

	template<class T>
	concept is_pawn = std::derived_from<T, APawn>;

	template<class T>
	concept is_controller = std::derived_from<T, AController>;

	template<class T>
	concept is_player_controller = std::derived_from<T, APlayerController>;

	template<class T>
	concept is_player_camera_manager = std::derived_from<T, APlayerCameraManager>;

	template<class T>
	concept is_local_player = std::derived_from<T, ULocalPlayer>;

	template<class T>
	concept is_ai_controller = std::derived_from<T, AAIController>;

	template<class T>
	concept is_game_state = std::derived_from<T, AGameStateBase>;

	template<class T>
	concept is_game_mode = std::derived_from<T, AGameModeBase>;

	template<class T>
	concept is_data_asset = std::derived_from<T, UDataAsset>;

	template<class T>
	concept is_curve_base = std::derived_from<T, UCurveBase>;

	template<class T>
	concept is_curve_float = std::derived_from<T, UCurveFloat>;

	template<class T>
	concept is_curve_vector = std::derived_from<T, UCurveVector>;

	template<class T>
	concept is_curve_linear_color = std::derived_from<T, UCurveLinearColor>;

	template<class T>
	concept is_primary_data_asset = std::derived_from<T, UPrimaryDataAsset>;

	template<class T>
	concept is_anim_instance = std::derived_from<T, UAnimInstance>;

	template<class T>
	concept is_material_interface = std::derived_from<T, UMaterialInterface>;

	template<class T>
	concept is_streamable_render_asset = std::derived_from<T, UStreamableRenderAsset>;

	template<class T>
	concept is_meadia_texture = std::derived_from<T, UMediaTexture>;

	template<class T>
	concept is_slate_texture_atlas_interface = std::derived_from<T, ISlateTextureAtlasInterface>;

	template<class T>
	concept is_image = !is_meadia_texture<T> && (is_material_interface<T> || is_slate_texture_atlas_interface<T>);

	template<class T>
	concept is_property = std::derived_from<T, FProperty>;

	template<class T>
	concept is_object_property_base = std::derived_from<T, FObjectPropertyBase>;

	template<class T>
	concept is_soft_object_property = std::derived_from<T, FSoftObjectProperty>;

	template<class T>
	concept is_table_row = std::derived_from<T, FTableRowBase>;

	template<class T>
	concept is_animation_asset = std::derived_from<T, UAnimationAsset>;

	template<class T>
	concept is_anim_sequence_base = std::derived_from<T, UAnimSequenceBase>;

	template<class T>
	concept is_anim_sequence = std::derived_from<T, UAnimSequence>;

	template<class T>
	concept is_anim_montage = std::derived_from<T, UAnimMontage>;

	template<class T>
	concept is_blend_space = std::derived_from<T, UBlendSpace>;

	template<class T>
	concept is_aim_offset = std::derived_from<T, UAimOffsetBlendSpace>;

	template<class T>
	concept is_aim_composite = std::derived_from<T, UAnimComposite>;

	template<class T>
	concept is_aim_streamable = std::derived_from<T, UAnimStreamable>;

	template<class T>
	concept is_pose_asset = std::derived_from<T, UPoseAsset>;

	template<class T>
	concept is_camera_modifier = std::derived_from<T, UCameraModifier>;
}
