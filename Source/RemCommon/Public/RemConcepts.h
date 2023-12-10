
#pragma once

namespace Rem::Common::Concepts
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
	concept lex_to_string = requires (FString String, const T Object)
	{
		String = LexToString(Object);
	};

	template<class T>
	concept unreal_struct_provider = requires (UScriptStruct*& StructRef)
	{
		StructRef = T::StaticStruct();
	};
	
	template<class T>
	concept is_stringable = unreal_struct_provider<T>
		|| std::is_enum_v<T>
		|| std::is_same_v<bool, T>
		|| has_to_string<T>
		|| has_get_name<T>
		|| lex_to_string<T>;

	template<class T>
	concept unreal_class_provider = requires (UClass*& StructRef)
	{
		StructRef = T::StaticClass();
	};

	template<class T>
	concept has_is_valid = requires (bool Result, const T Object)
	{
		Result = Object.IsValid();
	};

	template<class T>
	concept is_uobject = std::is_base_of_v<UObject, T>;

	template<class T>
	concept is_data_asset = std::is_base_of_v<UDataAsset, T>;

	template<class T>
	concept is_anim_instance = std::is_base_of_v<UAnimInstance, T>;
}
