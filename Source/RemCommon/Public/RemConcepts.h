// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include "Engine/EngineBaseTypes.h"

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
	concept is_uobject = std::is_base_of_v<UObject, T>;

	template<class T>
	concept is_uclass = std::is_base_of_v<UClass, T>;

	template<class T>
	concept is_hud = std::is_base_of_v<AHUD, T>;

	template<class T>
	concept is_actor = std::is_base_of_v<AActor, T>;

	template<class T>
	concept is_actor_component = std::is_base_of_v<UActorComponent, T>;

	template<class T>
	concept is_player_state = std::is_base_of_v<APlayerState, T>;

	template<class T>
	concept is_pawn = std::is_base_of_v<APawn, T>;

	template<class T>
	concept is_controller = std::is_base_of_v<AController, T>;

	template<class T>
	concept is_player_controller = std::is_base_of_v<APlayerController, T>;

	template<class T>
	concept is_player_camera_manager = std::is_base_of_v<APlayerCameraManager, T>;

	template<class T>
	concept is_local_player = std::is_base_of_v<ULocalPlayer, T>;

	template<class T>
	concept is_ai_controller = std::is_base_of_v<AAIController, T>;

	template<class T>
	concept is_game_state = std::is_base_of_v<AGameStateBase, T>;

	template<class T>
	concept is_game_mode = std::is_base_of_v<AGameModeBase, T>;

	template<class T>
	concept is_data_asset = std::is_base_of_v<UDataAsset, T>;

	template<class T>
	concept is_curve_base = std::is_base_of_v<UCurveBase, T>;

	template<class T>
	concept is_curve_float = std::is_base_of_v<UCurveFloat, T>;

	template<class T>
	concept is_curve_vector = std::is_base_of_v<UCurveVector, T>;

	template<class T>
	concept is_curve_linear_color = std::is_base_of_v<UCurveLinearColor, T>;

	template<class T>
	concept is_primary_data_asset = std::is_base_of_v<UPrimaryDataAsset, T>;

	template<class T>
	concept is_anim_instance = std::is_base_of_v<UAnimInstance, T>;

	template<class T>
	concept is_material_interface = std::is_base_of_v<UMaterialInterface, T>;

	template<class T>
	concept is_streamable_render_asset = std::is_base_of_v<UStreamableRenderAsset, T>;

	template<class T>
	concept is_meadia_texture = std::is_base_of_v<UMediaTexture, T>;

	template<class T>
	concept is_slate_texture_atlas_interface = std::is_base_of_v<ISlateTextureAtlasInterface, T>;

	template<class T>
	concept is_image = !is_meadia_texture<T> && (is_material_interface<T> || is_slate_texture_atlas_interface<T>);

	template<class T>
	concept is_property = std::is_base_of_v<FProperty, T>;

	template<class T>
	concept is_object_property_base = std::is_base_of_v<FObjectPropertyBase, T>;

	template<class T>
	concept is_soft_object_property = std::is_base_of_v<FSoftObjectProperty, T>;

	template<class T>
	concept is_table_row = std::is_base_of_v<FTableRowBase, T>;

	template<class T>
	concept is_animation_asset = std::is_base_of_v<UAnimationAsset, T>;

	template<class T>
	concept is_anim_sequence_base = std::is_base_of_v<UAnimSequenceBase, T>;

	template<class T>
	concept is_anim_sequence = std::is_base_of_v<UAnimSequence, T>;

	template<class T>
	concept is_anim_montage = std::is_base_of_v<UAnimMontage, T>;

	template<class T>
	concept is_blend_space = std::is_base_of_v<UBlendSpace, T>;

	template<class T>
	concept is_aim_offset = std::is_base_of_v<UAimOffsetBlendSpace, T>;

	template<class T>
	concept is_aim_composite = std::is_base_of_v<UAnimComposite, T>;

	template<class T>
	concept is_aim_streamable = std::is_base_of_v<UAnimStreamable, T>;

	template<class T>
	concept is_pose_asset = std::is_base_of_v<UPoseAsset, T>;

	template<class T>
	concept is_camera_modifier = std::is_base_of_v<UCameraModifier, T>;
}
