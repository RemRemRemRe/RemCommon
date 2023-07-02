// Copyright . All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include <string_view>

namespace Rem::Common
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
			else if constexpr (Concepts::has_is_valid<RawType>)
			{
				return Object->IsValid();
			}
			else
			{
				return Object != nullptr;
			}
		}
		else
		{
			if constexpr (TIsTObjectPtr<T>::Value)
			{
				return Common::IsValid(Object.Get());
			}
			else if constexpr (Concepts::has_is_valid<T>)
			{
				return Object.IsValid();
			}
			else
			{
				static_assert(std::_Always_false<T>, "T is either a TObjectPtr nor having 'IsValid' member");
				return false;
			}
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
			return Common::GetNetMode(Object.Get());
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
			return Common::IsNetMode(Object.Get(), NetMode);
		}
		else
		{
			static_assert(std::_Always_false<T>, "T should be pointer or TObjectPtr");
			return {};
		}
	}

	REMCOMMON_API bool IsClassDefaultObject(const UObject* Object);

	constexpr FStringView BoolToString(const bool bValue)
	{
		return bValue ? TEXTVIEW("True") : TEXTVIEW("False");
	}
	
	REMCOMMON_API FString GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath);

	template<typename T>
	FString ToString(T&& Data)
	{
		using RawType = std::_Remove_cvref_t<T>;

		// pointer
		if constexpr (std::is_pointer_v<RawType>)
		{
			using NoPointerRawType = std::remove_pointer_t<RawType>;
			
			if constexpr (Concepts::has_to_string<NoPointerRawType>)
			{
				return Data->ToString();
			}
			else if constexpr (Concepts::has_get_name<NoPointerRawType>)
			{
				return Data->GetName();
			}
			else if constexpr (Concepts::unreal_struct_provider<NoPointerRawType>)
			{
				FString HumanReadableMessage;
				NoPointerRawType::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, Data,
					/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
				return HumanReadableMessage;
			}
			else
			{
				static_assert(std::_Always_false<T>, "T is not supported for ToString");
				return {};
			}
		}
		else // value
		{
			if constexpr (std::is_enum_v<RawType>)
			{
				return UEnum::GetValueAsString(std::forward<T>(Data));
			}
			else if constexpr (std::is_same_v<bool, RawType>)
			{
				return Common::BoolToString(std::forward<T>(Data));
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
				FString HumanReadableMessage;
				RawType::StaticStruct()->ExportText(/*out*/ HumanReadableMessage, &Data,
					/*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
				return HumanReadableMessage;
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

	template<typename T, bool bConstantStringLength = false>
	FString GetNetModeString(const T* Object)
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
		
		return FString::Format(TEXT("Client {0}"), {GPlayInEditorID - 1});
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
	FString GetNetRoleString(const T* Object)
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
	FName GetNetRoleName(const T* Object)
	{
		return StaticEnum<ENetRole>()->GetValueAsName(Common::GetNetRole(Object));
	}

	template<typename T>
	FText GetNetRoleText(const T* Object)
	{
		return StaticEnum<ENetRole>()->GetDisplayValueAsText(Common::GetNetRole(Object));
	}

	template<typename  T>
	FString GetNetDebugString(T* Object)
	{
		if (Common::IsValid(Object))
		{
			const FString NetModeString = Common::GetNetModeString<T, true>(Object);

			if constexpr (Concepts::has_get_local_role<T> || Concepts::has_get_owner_role<T>)
			{
				const FString NetRoleString = Common::GetNetRoleString<T, true>(Object);
								
				return FString::Format(TEXT("{0} {1}"), { NetModeString, NetRoleString });
			}
			else
			{
				return FString::Format(TEXT("{0}"), { NetModeString });
			}
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
}
