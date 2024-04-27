// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

class AAIController;
class ISlateTextureAtlasInterface;
class ULocalPlayer;
class AGameStateBase;
class AGameModeBase;
class UAnimInstance;
class UStreamableRenderAsset;
struct FTableRowBase;

namespace Rem::Concepts
{
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
	concept has_is_net_mode = requires (ENetMode Mode, const T Object, bool Result)
	{
		Result = Object.IsNetMode(Mode);
	};

	template<class T>
	concept has_to_compact_string = requires (FString String, const T Object)
	{
		String = Object.ToCompactString();
	};

	template<class T>
	concept has_to_string = requires (FString String, const T Object)
	{
		String = Object.ToString();
	};

	template<class T>
	concept has_get_name = requires (FString String, const T Object)
	{
		String = Object.GetName();
	};

	template<class T>
	concept has_lex_to_string = requires (FString String, const T Object)
	{
		String = LexToString(Object);
	};

	template<class T>
	concept has_static_struct = requires (UScriptStruct*& StructRef)
	{
		StructRef = T::StaticStruct();
	};
	
	template<class T>
	concept is_stringable = has_static_struct<T>
		|| std::is_enum_v<T>
		|| std::is_same_v<bool, T>
		|| has_to_string<T>
		|| has_get_name<T>
		|| has_lex_to_string<T>;

	template<class T>
	concept has_static_class = requires (UClass*& StructRef)
	{
		StructRef = T::StaticClass();
	};

	template<class T>
	concept has_is_valid = requires (bool Result, const T Object)
	{
		Result = Object.IsValid();
	};

	template<class T>
	concept has_to_soft_object_path = requires (FSoftObjectPath SoftPath, bool bNull, const T Object)
	{
		SoftPath = Object.ToSoftObjectPath();
	};

	template<class T>
	concept is_uobject = std::is_base_of_v<UObject, T>;

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
	concept is_anim_instance = std::is_base_of_v<UAnimInstance, T>;

	template<class T>
	concept is_material_interface = std::is_base_of_v<UMaterialInterface, T>;

	template<class T>
	concept is_streamable_render_asset = std::is_base_of_v<UStreamableRenderAsset, T>;

	template<class T>
	concept is_image = is_material_interface<T> || std::is_base_of_v<ISlateTextureAtlasInterface, T>;

	template<class T>
	concept is_property = std::is_base_of_v<FProperty, T>;

	template<class T>
	concept is_object_property_base = std::is_base_of_v<FObjectPropertyBase, T>;

	template<class T>
	concept is_soft_object_property = std::is_base_of_v<FSoftObjectProperty, T>;

	template<class T>
	concept is_table_row = std::is_base_of_v<FTableRowBase, T>;
}
