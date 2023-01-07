// Copyright . All Rights Reserved.

#pragma once

#include "Concepts.h"

#if WITH_EDITOR

#define RETURN_IF_NOT_GAME_WORLD \
	if (!GetWorld() || !GetWorld()->IsGameWorld()) \
	{ \
		return; \
	}

#else

#define RETURN_IF_NOT_GAME_WORLD

#endif

namespace Common
{
	template<typename T>
	bool IsValid(const T Object)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			using RawType = std::remove_pointer_t<T>;
			
			if constexpr (std::is_base_of_v<UObject, RawType>)
			{
				return ::IsValid(Object);
			}
			else
			{
				return Object != nullptr;
			}
		}
		
		else if constexpr (TIsTObjectPtr<T>::Value)
		{
			return IsValid(Object.Get());
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should be pointer or TObjectPtr");
			return false;
		}
	}

	template<typename T>
	ENetMode GetNetMode(const T Object)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			using RawType = std::remove_pointer_t<T>;
			
			if constexpr (Concepts::has_get_net_mode<RawType>)
			{
				return Object->GetNetMode();
			}
			else
			{
				static_assert(std::_Always_false<T>, "T should have member GetNetMode");
				return {};
			}
		}

		else if constexpr (TIsTObjectPtr<T>::Value)
		{
			return GetNetMode(Object.Get());
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should be pointer or TObjectPtr");
			return {};
		}
	}

	template<typename T>
	bool IsNetMode(const T Object, const ENetMode NetMode)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			using RawType = std::remove_pointer_t<T>;
			
			if constexpr (Concepts::has_is_net_mode<RawType>)
			{
				return Object->IsNetMode(NetMode);
			}
			else if constexpr (Concepts::has_get_net_mode<RawType>)
			{
				return Object->GetNetMode() == NetMode;
			}
			else
			{
				static_assert(std::_Always_false<T>, "T should have member IsNetMode or GetNetMode");
				return {};
			}
		}
		
		else if constexpr (TIsTObjectPtr<T>::Value)
		{
			return IsNetMode(Object.Get(), NetMode);
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should be pointer or TObjectPtr");
			return {};
		}
	}

	COMMON_API bool IsClassDefaultObject(const UObject* Object);

	COMMON_API FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	template<typename T>
	FString ToString(const T& Data)
	{
		if constexpr (Concepts::has_to_string<T>)
		{
			return Data.ToString();
		}
		else if constexpr (Concepts::unreal_struct_provider<T>)
		{
			FString HumanReadableMessage;
			T::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, Data,
				/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
			return HumanReadableMessage;
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should have member ToString or static member StaticStruct");
			return {};
		}
	}
	
	template<typename T>
	ENetRole GetNetRole(const T* Object)
	{
		if constexpr (Concepts::has_get_local_role<T>)
		{
			return Object->GetLocalRole();
		}
		else if constexpr (Concepts::has_get_owner_role<T>)
		{
			return Object->GetOwnerRole();
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should have member GetLocalRole or GetOwnerRole that returns ENetRole");
			return {};
		}
	}

	template<typename T>
	FString GetNetRoleString(const T* Object)
	{
		return StaticEnum<ENetRole>()->GetValueAsString(GetNetRole(Object));
	}

	template<typename T>
	FName GetNetRoleName(const T* Object)
	{
		return StaticEnum<ENetRole>()->GetValueAsName(GetNetRole(Object));
	}

	template<typename T>
	FText GetNetRoleText(const T* Object)
	{
		return StaticEnum<ENetRole>()->GetDisplayValueAsText(GetNetRole(Object));
	}
}
