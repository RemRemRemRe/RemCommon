
#pragma once

namespace Common::Concepts
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
	concept unreal_struct_provider = requires (UScriptStruct*& StructRef)
	{
		StructRef = T::StaticStruct();
	};

	template<class T>
	concept unreal_class_provider = requires (UClass*& StructRef)
	{
		StructRef = T::StaticClass();
	};
}
