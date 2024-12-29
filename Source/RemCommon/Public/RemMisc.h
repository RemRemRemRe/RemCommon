// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include <type_traits>

#include "RemAlwaysFalse.h"
#include "RemConcepts.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Engine/EngineBaseTypes.h"

struct FRealCurve;
struct FGameplayTag;
class IConsoleVariable;
template <typename T>
struct TIsUEnumClass;
class FDelegateHandle;

namespace Rem
{
	template<typename T>
	bool IsValid(const T& Object)
	{
		using RawType = std::remove_cvref_t<T>;

		if constexpr (std::is_pointer_v<T>)
		{
			if (Object != nullptr)
			{
				using Type = std::remove_pointer_t<RawType>;

				if constexpr (std::is_base_of_v<UObject, Type>)
				{
					return Rem::IsValid(*Object);
				}
				else
				{
					return true;
				}
			}
			return {};
		}
		else
		{
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
				static_assert(always_false<T>::value, "T is either a TObjectPtr, UObject nor has 'IsValid' member");
				return false;
			}
		}
	}

	template<typename T>
	ENetMode GetNetMode(const T& Object)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::GetNetMode(*Object);
		}
		else
		{
			using RawType = std::remove_cvref_t<T>;

			if constexpr (TIsTObjectPtr<RawType>::Value)
			{
				return Rem::GetNetMode(*Object);
			}
			else if constexpr (Concepts::has_get_net_mode<RawType>)
			{
				return Object.GetNetMode();
			}
			else
			{
				static_assert(always_false<T>::value, "T should have member GetNetMode");
				return {};
			}
		}
	}

	template<typename T>
	bool IsNetMode(const T& Object, const ENetMode NetMode)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::IsNetMode(*Object);
		}
		else
		{
			using RawType = std::remove_cvref_t<T>;

			if constexpr (TIsTObjectPtr<RawType>::Value)
			{
				return Rem::IsNetMode(*Object, NetMode);
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
				static_assert(always_false<T>::value, "T should have member IsNetMode or GetNetMode");
				return {};
			}
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
	FString ToString(const T& Data)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::ToString(*Data);
		}
		else
		{
			using RawType = std::remove_cvref_t<T>;

			if constexpr (std::is_same_v<bool, RawType>)
			{
				return Rem::BoolToString(Data).GetData();
			}
			else if constexpr (Concepts::has_to_compact_string<RawType>)
			{
				return Data.ToCompactString();
			}
			else if constexpr (Concepts::has_to_string_simple<RawType>)
			{
				return Data.ToStringSimple();
			}
			else if constexpr (Concepts::has_to_simple_string<RawType>)
			{
				return Data.ToSimpleString();
			}
			else if constexpr (Concepts::has_to_string<RawType>)
			{
				return Data.ToString();
			}
			else if constexpr (Concepts::has_lex_to_string<RawType>)
			{
				return LexToString(Data);
			}
			else if constexpr (Concepts::has_get_name<RawType>)
			{
				return Data.GetName();
			}
			else if constexpr (TIsTObjectPtr<RawType>::Value)
			{
				return Rem::ToString(*Data);
			}
			else if constexpr (std::is_same_v<FDelegateHandle, RawType>)
			{
				// print FDelegateHandle as uint64
				return FString::Format(TEXT("{0}"), {*reinterpret_cast<const uint64*>(&Data)});
			}
			else if constexpr (TIsUEnumClass<RawType>::Value)
			{
				return UEnum::GetValueAsString(Data);
			}
			else if constexpr (Concepts::has_static_struct<RawType>)
			{
				return ToString(*RawType::StaticStruct(), &Data);
			}
			else
			{
				static_assert(always_false<T>::value, "T is not supported for ToString");
				return {};
			}
		}
	}

	template<typename T>
	ENetRole GetNetRole(const T& Object)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::GetNetRole(*Object);
		}
		else
		{
			using RawType = std::remove_cvref_t<T>;

			if constexpr (TIsTObjectPtr<RawType>::Value)
			{
				return Rem::GetNetRole(*Object);
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
				static_assert(always_false<T>::value, "T should have member GetLocalRole or GetOwnerRole that returns ENetRole");
				return {};
			}
		}
	}

	template<typename T, bool bConstantStringLength = false>
	FString GetNetModeString(const T& Object)
	{
		if (Rem::IsNetMode(Object, NM_DedicatedServer) || Rem::IsNetMode(Object, NM_ListenServer))
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

		return FString::Format(TEXT("Client:{0}"), {UE::GetPlayInEditorID()});
	}

	/**
	 * @brief Append spaces to the string
	 * @note better to make sure String has enough capacity using FString::Reserve
	 */
	REMCOMMON_API void AppendCharRepeated(FString& String, const TCHAR Char, const int32 TimesToRepeat);

	template<typename T, bool bConstantStringLength = false>
	FString GetNetRoleString(const T& Object)
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::GetNetRoleString(*Object);
		}
		else
		{
			FString NetRoleString = StaticEnum<ENetRole>()->GetValueAsString(Rem::GetNetRole(Object));

			if constexpr (bConstantStringLength)
			{
				constexpr auto MaxLength = TEXTVIEW("ROLE_AutonomousProxy").Len();

				NetRoleString.Reserve(MaxLength);
				AppendCharRepeated(NetRoleString, TEXT(' '), MaxLength - NetRoleString.Len());
			}

			return NetRoleString;
		}
	}

	template<typename T>
	FName GetNetRoleName(const T& Object)
	{
		return StaticEnum<ENetRole>()->GetValueAsName(Rem::GetNetRole(Object));
	}

	template<typename T>
	FText GetNetRoleText(const T& Object)
	{
		return StaticEnum<ENetRole>()->GetDisplayValueAsText(Rem::GetNetRole(Object));
	}

	template<typename  T>
	FString GetNetDebugString(const T& Object)
	{
		if (!Rem::IsValid(Object))
		{
			return TEXT("Invalid Object");
		}

		if constexpr (std::is_pointer_v<T>)
		{
			return Rem::GetNetDebugString(*Object);
		}
		else
		{
			using RawType = std::remove_cvref_t<T>;

			if constexpr (TIsTObjectPtr<RawType>::Value)
			{
				return Rem::GetNetDebugString(*Object);
			}
			else
			{
				const FString NetModeString = Rem::GetNetModeString<T, true>(Object);

				if constexpr (Concepts::has_get_local_role<T> || Concepts::has_get_owner_role<T>)
				{
					const FString NetRoleString = Rem::GetNetRoleString<T, true>(Object);

					return FString::Format(TEXT("{0} {1}"), { NetModeString, NetRoleString });
				}
				else
				{
					return FString::Format(TEXT("{0}"), { NetModeString });
				}
			}
		}
	}

	namespace Impl
	{
		// recursion ends with this empty implementation:
		inline void FillStringFormatArgs(FStringFormatOrderedArguments&)
		{
		}

		template<typename F, typename... R>
		void FillStringFormatArgs(FStringFormatOrderedArguments& Args, F&& First, R&&... Rest)
		{
			if constexpr (!std::is_same_v<bool, F> && std::is_constructible_v<FStringFormatArg, F>)
			{
				Args.Add(std::forward<F>(First));
			}
			else
			{
				Args.Add(Rem::ToString(std::forward<F>(First)));
			}

			Impl::FillStringFormatArgs(Args, std::forward<R>(Rest)...);
		}
	}

	template<typename... T>
	FString StringFormat(const TCHAR* Format, T&&... Args)
	{
		FStringFormatOrderedArguments OrderedArgs;
		Impl::FillStringFormatArgs(OrderedArgs, std::forward<T>(Args)...);

		return FString::Format(Format, std::move(OrderedArgs));
	}

	template<Concepts::is_uobject T>
	decltype(auto) GetDefaultRef()
	{
		 return *::GetDefault<T>();
	}

	template<Concepts::is_uobject T>
	decltype(auto) GetMutableDefaultRef()
	{
		 return *::GetMutableDefault<T>();
	}

	constexpr uint8 RoleCount = ENetRole::ROLE_MAX - 1u;
	constexpr uint8 NetModeCount = ENetMode::NM_MAX;

	REMCOMMON_API uint8* AllocateStructMemory(const UStruct& Struct);
	REMCOMMON_API void FreeStructMemory(const UStruct& Struct, uint8& Memory);

	REMCOMMON_API IConsoleVariable* FindConsoleVariable(const TCHAR* Name);

	REMCOMMON_API float EvaluateCurve01(const FRealCurve& RichCurve, const float Alpha);
	REMCOMMON_API float EvaluateCurve01Clamped(const FRealCurve& RichCurve, const float Alpha);
}
