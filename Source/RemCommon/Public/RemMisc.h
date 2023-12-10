﻿// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemConcepts.h"

namespace Rem::Common
{
	template<typename T>
	bool IsValid(T&& Object)
	{
		using RawType = std::remove_reference_t<T>;

		if constexpr (std::is_base_of_v<UObject, RawType>)
		{
			return ::IsValidChecked(&Object);
		}
		else if constexpr (TIsTObjectPtr<RawType>::Value)
		{
			return ::IsValid(Object.Get());
		}
		else if constexpr (Concepts::has_is_valid<RawType>)
		{
			return Object.IsValid();
		}
		else
		{
			static_assert(std::_Always_false<T>, "T is either a TObjectPtr, UObject nor having 'IsValid' member");
			return false;
		}
	}

	template<typename T>
	bool IsValid(T* Object)
	{
		if (Object != nullptr)
		{
			using RawType = std::remove_reference_t<T>;
			
			if constexpr (std::is_base_of_v<UObject, RawType>)
			{
				return Common::IsValid(*Object);
			}
			else
			{
				return true;
			}
		}
		return {};
	}

	template<typename T>
	ENetMode GetNetMode(T&& Object)
	{
		using RawType = std::remove_reference_t<T>;
		
		if constexpr (TIsTObjectPtr<RawType>::Value)
		{
			return Common::GetNetMode(*Object);
		}
		else if constexpr (Concepts::has_get_net_mode<RawType>)
		{
			return Object.GetNetMode();
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should have member GetNetMode");
			return {};
		}
	}

	template<typename T>
	bool IsNetMode(T&& Object, const ENetMode NetMode)
	{
		using RawType = std::remove_reference_t<T>;
			
		if constexpr (TIsTObjectPtr<RawType>::Value)
		{
			return Common::IsNetMode(*Object, NetMode);
		}
		else if constexpr (Concepts::has_is_net_mode<RawType>)
		{
			return Object.IsNetMode(NetMode);
		}
		else if constexpr (Concepts::has_get_net_mode<RawType>)
		{
			return Object.GetNetMode() == NetMode;
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should have member IsNetMode or GetNetMode");
			return {};
		}
	}

	REMCOMMON_API bool IsClassDefaultObject(const UObject* Object);

	constexpr FStringView BoolToString(const bool bValue)
	{
		return bValue ? TEXTVIEW("True") : TEXTVIEW("False");
	}
	
	REMCOMMON_API FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	REMCOMMON_API FString ToString(const UScriptStruct& ScriptStruct, const void* Value);

	template<typename T>
	FString ToString(T&& Data)
	{
		using RawType = std::remove_reference_t<T>;
		
		if constexpr (std::is_enum_v<RawType>)
		{
			return UEnum::GetValueAsString(Data);
		}
		else if constexpr (std::is_same_v<bool, RawType>)
		{
			return Common::BoolToString(std::forward<T>(Data));
		}
		else if constexpr (TIsTObjectPtr<RawType>::Value)
		{
			return Common::ToString(*Data);
		}
		else if constexpr (Concepts::has_to_string<RawType>)
		{
			return Data.ToString();
		}
		else if constexpr (Concepts::has_get_name<RawType>)
		{
			return Data.GetName();
		}
		else if constexpr (Concepts::unreal_struct_provider<RawType>)
		{
			return ToString(*RawType::StaticStruct(), &Data);
		}
		else if constexpr (Concepts::lex_to_string<RawType>)
		{
			return LexToString(std::forward<T>(Data));
		}
		else
		{
			static_assert(std::_Always_false<T>, "T is not supported for ToString");
			return {};
		}
	}

	template<typename T>
	FString ToString(T* Data)
	{
		if (Common::IsValid(Data))
		{
			return ToString(*Data);
		}
		return {};
	}
	
	template<typename T>
	ENetRole GetNetRole(T&& Object)
	{
		using RawType = std::remove_reference_t<T>;
		
		if constexpr (TIsTObjectPtr<RawType>::Value)
		{
			return Common::GetNetRole(*Object);
		}
		if constexpr (Concepts::has_get_local_role<T>)
		{
			return Object.GetLocalRole();
		}
		else if constexpr (Concepts::has_get_owner_role<T>)
		{
			return Object.GetOwnerRole();
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should have member GetLocalRole or GetOwnerRole that returns ENetRole");
			return {};
		}
	}

	template<typename T, bool bConstantStringLength = false>
	FString GetNetModeString(T&& Object)
	{
		if (Common::IsNetMode(Object, NM_DedicatedServer) || Common::IsNetMode(Object, NM_ListenServer))
		{
			if constexpr (bConstantStringLength)
			{
				return FString(TEXT("Server  "));
			}
			else
			{
				return FString(TEXT("Server"));
			}
		}
		
		return FString::Format(TEXT("Client {0}"), {int32{GPlayInEditorID}});
	}

	/**
	 * @brief Append spaces to the string
	 * @note better to make sure String has enough capacity using FString::Reserve
	 */
	inline void AppendSpaces(FString& String, const int32 NumSpaceToAdd)
	{
		for (int32 Count = 0; Count < NumSpaceToAdd; ++Count)
		{
			String.AppendChar(' ');
		}
	}
	
	template<typename T, bool bConstantStringLength = false>
	FString GetNetRoleString(T&& Object)
	{
		if constexpr (bConstantStringLength)
		{
			constexpr int32 MaxLength = std::string_view("ROLE_AutonomousProxy").length();
			
			FString NetRoleString = StaticEnum<ENetRole>()->GetValueAsString(Common::GetNetRole(Object));
			
			NetRoleString.Reserve(MaxLength);
			AppendSpaces(NetRoleString, MaxLength - NetRoleString.Len());
			
			return NetRoleString;
		}
		else
		{
			return StaticEnum<ENetRole>()->GetValueAsString(Common::GetNetRole(Object));
		}
	}

	template<typename T>
	FName GetNetRoleName(T&& Object)
	{
		return StaticEnum<ENetRole>()->GetValueAsName(Common::GetNetRole(Object));
	}

	template<typename T>
	FText GetNetRoleText(T&& Object)
	{
		return StaticEnum<ENetRole>()->GetDisplayValueAsText(Common::GetNetRole(Object));
	}

	template<typename  T, bool bShouldValidate = true>
	FString GetNetDebugString(T&& Object)
	{
		if constexpr (bShouldValidate)
		{
			if (!Common::IsValid(Object))
			{
				return {};
			}
		}
		
		const FString NetModeString = Common::GetNetModeString<T, true>(std::forward<T>(Object));

		if constexpr (Concepts::has_get_local_role<T> || Concepts::has_get_owner_role<T>)
		{
			const FString NetRoleString = Common::GetNetRoleString<T, true>(std::forward<T>(Object));
							
			return FString::Format(TEXT("{0} {1}"), { NetModeString, NetRoleString });
		}
		else
		{
			return FString::Format(TEXT("{0}"), { NetModeString });
		}
	}

	template<typename  T>
	FString GetNetDebugString(T* Object)
	{
		if (Common::IsValid(Object))
		{
			return GetNetDebugString<T, false>(std::forward<T>(*Object));
		}
		return {};
	}

	namespace Private
	{
		// recursion ends with this empty implementation:
		inline void FillStringFormatArgs(FStringFormatOrderedArguments&)
		{
		}

		template<typename F, typename... R>
		void FillStringFormatArgs(FStringFormatOrderedArguments& Args, F&& First, R&&... Rest)
		{
			if constexpr (std::is_constructible_v<FStringFormatArg, F>)
			{
				Args.Add(std::forward<F>(First));
			}
			else
			{
				Args.Add(Common::ToString(std::forward<F>(First)));
			}

			Private::FillStringFormatArgs(Args, std::forward<R>(Rest)...);
		}
	}

	template<typename... T>
	FString StringFormat(const TCHAR* Format, T&&... Args)
	{
		FStringFormatOrderedArguments OrderedArgs;
		Private::FillStringFormatArgs(OrderedArgs, std::forward<T>(Args)...);

		return FString::Format(Format, std::move(OrderedArgs));
	}

	template<typename T>
	requires std::is_base_of_v<UObject, T>
	decltype(auto) GetDefaultRef()
	{
		 return *::GetDefault<T>();
	}

	template<typename T>
	requires std::is_base_of_v<UObject, T>
	decltype(auto) GetMutableDefaultRef()
	{
		 return *::GetMutableDefault<T>();
	}

	constexpr uint8 RoleCount = ENetRole::ROLE_MAX - 1u;
	constexpr uint8 NetModeCount = ENetMode::NM_MAX;
}
