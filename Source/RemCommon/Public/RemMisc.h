// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>
#include <string_view>

#include "fmt/format.h"

#include "RemAlwaysFalse.h"
#include "RemConcepts.h"
#include "RemNotNull.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "Containers/AnsiString.h"

struct FRealCurve;
struct FGameplayTag;
class IConsoleVariable;
template <typename T>
struct TIsUEnumClass;
class FDelegateHandle;

namespace Rem
{
template <typename T>
bool IsValid(const T& Object)
{
    using RawType = std::remove_cvref_t<T>;

    if constexpr (std::is_pointer_v<RawType>)
    {
        if (Object != nullptr)
        {
            using Type = std::remove_pointer_t<RawType>;

            if constexpr (std::derived_from<Type, UObject>)
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
    else if constexpr (CNotNull<RawType>)
    {
        return Rem::IsValid(*Object);
    }
    else
    {
        if constexpr (std::derived_from<RawType, UObject>)
        {
            return ::IsValidChecked(&Object);
        }
        else if constexpr (TIsTObjectPtr<RawType>::Value)
        {
            return ::IsValid(Object.Get());
        }
        else if constexpr (CHasIsValid<RawType>)
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

REM_FUNCTION_TO_FUNCTOR_SIMPLE(IsValid)

template <typename T>
ENetMode GetNetMode(const T& Object)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
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
        else if constexpr (CHasGetNetMode<RawType>)
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

template <typename T>
bool IsNetMode(const T& Object, const ENetMode NetMode)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
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
        else if constexpr (CHasIsNetMode<RawType>)
        {
            return Object.IsNetMode(NetMode);
        }
        else if constexpr (CHasGetNetMode<RawType>)
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

constexpr FAnsiStringView BoolToString(const bool bValue)
{
    return bValue
               ? ANSITEXTVIEW("True")
               : ANSITEXTVIEW("False");
}

template <typename TCharType>
void GetObjectNameFromSoftObjectPath(TStringBuilderBase<TCharType>& Builder, const FSoftObjectPath& SoftObjectPath)
{
    const auto& SubPathString = SoftObjectPath.GetSubPathUtf8String();

    if (int32 ObjectNameIndex;
        SubPathString.FindLastChar(TEXT('.'), ObjectNameIndex))
    {
        Builder << SubPathString.RightChop(ObjectNameIndex + 1);
    }
}

inline FUtf8String GetObjectNameFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath)
{
    TUtf8StringBuilder<64> Builder;
    Rem::GetObjectNameFromSoftObjectPath(Builder, SoftObjectPath);
    return FUtf8String(Builder);
}

template <typename TCharType>
void PointerToString(TStringBuilderBase<TCharType>& Builder, const TNotNull<const void*> Value)
{
    Builder << FAnsiString::Printf("%p", NotNullGet(Value));
}

inline FUtf8String PointerToString(const TNotNull<const void*> Value)
{
    TUtf8StringBuilder<64> Builder;
    Rem::PointerToString(Builder, Value);
    return FUtf8String(Builder);
}

template <typename TCharType>
void ToString(TStringBuilderBase<TCharType>& Builder, const TNotNull<const UScriptStruct*> ScriptStruct,
    const TNotNull<const void*> Value)
{
    FString HumanReadableMessage;
    ScriptStruct->ExportText(/*out*/ HumanReadableMessage, Value,
        /*Defaults=*/ nullptr, /*OwnerObject=*/ nullptr, PPF_None, /*ExportRootScope=*/ nullptr);
    Builder << HumanReadableMessage;
}

inline FUtf8String ToString(const TNotNull<const UScriptStruct*> ScriptStruct, const TNotNull<const void*> Value)
{
    TUtf8StringBuilder<256> Builder;
    Rem::ToString(Builder, ScriptStruct, Value);
    return FUtf8String(Builder);
}

template <typename TCharType, typename T>
void ToString(TStringBuilderBase<TCharType>& Builder, const T& Data)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
    {
        Rem::ToString(Builder, *Data);
    }
    else
    {
        using RawType = std::remove_cvref_t<T>;

        if constexpr (std::is_same_v<bool, RawType>)
        {
            Builder << Rem::BoolToString(Data);
        }
        else if constexpr (CHasToUtf8StringOut<RawType>)
        {
            Data.ToUtf8String(Builder);
        }
        else if constexpr (CHasToStringOut<RawType>)
        {
            Data.ToString(Builder);
        }
        else if constexpr (CHasToUtf8String<RawType>)
        {
            Builder << Data.ToUtf8String();
        }
        else if constexpr (CHasToCompactString<RawType>)
        {
            Builder << Data.ToCompactString();
        }
        else if constexpr (CHasToStringSimple<RawType>)
        {
            Builder << Data.ToStringSimple();
        }
        else if constexpr (CHasToSimpleString<RawType>)
        {
            Builder << Data.ToSimpleString();
        }
        else if constexpr (CHasToString<RawType>)
        {
            Builder << Data.ToString();
        }
        else if constexpr (CHasGetName<RawType>)
        {
            Builder << Data.GetName();
        }
        else if constexpr (std::is_same_v<FDelegateHandle, RawType>)
        {
            Builder << *reinterpret_cast<const uint64*>(&Data);
        }
        else if constexpr (CUEnum<T> || TIsUEnumClass<RawType>::Value)
        {
            Builder << UEnum::GetValueAsString(Data);
        }
        else if constexpr (CHasStaticStruct<RawType>)
        {
            Rem::ToString(Builder, RawType::StaticStruct(), &Data);
        }
        else if constexpr (requires { Builder << Data; })
        {
            Builder << Data;
        }
        else if constexpr (CCanLexToString<RawType>)
        {
            Builder << LexToString(Data);
        }
        else
        {
            static_assert(always_false<T>::value, "T is not supported for ToString");
        }
    }
}

template <typename T>
FUtf8String ToString(const T& Data)
{
    TUtf8StringBuilder<256> Builder;
    Rem::ToString(Builder, Data);
    return FUtf8String(Builder);
}

namespace Private
{

template <typename TCharType>
struct TFormatOutputIterator
{
    TNotNull<TStringBuilderBase<TCharType>*> Builder;

    TFormatOutputIterator& operator*() { return *this; }
    TFormatOutputIterator& operator++() { return *this; }
    TFormatOutputIterator& operator++(int) { return *this; }

    TFormatOutputIterator& operator=(TCharType C)
    {
        Builder->AppendChar(C);
        return *this;
    }
};

template <typename T>
consteval bool IsCharacterPointerType()
{
    using Raw = std::remove_cvref_t<T>;
    if constexpr (std::is_pointer_v<Raw>)
    {
        using CVStripped = std::remove_cv_t<std::remove_pointer_t<Raw>>;
        return std::is_same_v<CVStripped, ANSICHAR>
               || std::is_same_v<CVStripped, WIDECHAR>
               || std::is_same_v<CVStripped, UTF8CHAR>;
    }
    return false;
}

template <typename T>
decltype(auto) ForwardFormatArg(T&& Arg)
{
    using Raw = std::remove_cvref_t<T>;
    if constexpr (std::is_pointer_v<Raw> && !std::is_void_v<std::remove_pointer_t<Raw>>
                  && !IsCharacterPointerType<T>())
    {
        return *Arg;
    }
    else
    {
        return std::forward<T>(Arg);
    }
}

template <typename T>
using TForwardedArg = std::conditional_t<
    std::is_rvalue_reference_v<decltype(Private::ForwardFormatArg(std::declval<T&&>()))>,
    std::remove_reference_t<decltype(Private::ForwardFormatArg(std::declval<T&&>()))>,
    decltype(Private::ForwardFormatArg(std::declval<T&&>()))>;

}


template <typename TCharType, typename... TArgs>
void Format(TStringBuilderBase<TCharType>& Builder, fmt::format_string<Private::TForwardedArg<TArgs>...> Fmt,
    TArgs&&... Args)
{
    auto Out = Private::TFormatOutputIterator<TCharType>{&Builder};
    fmt::format_to(Out, Fmt, Private::ForwardFormatArg(std::forward<TArgs>(Args))...);
}

template <typename... TArgs>
FUtf8String Format(fmt::format_string<Private::TForwardedArg<TArgs>...> Fmt, TArgs&&... Args)
{
    TUtf8StringBuilder<256> Builder;
    Rem::Format(Builder, Fmt, std::forward<TArgs>(Args)...);
    return FUtf8String(Builder);
}


template <typename T>
concept CToStringable = requires(T&& Object)
{
    Rem::ToString(std::forward<T>(Object));
};

template <typename T>
ENetRole GetNetRole(const T& Object)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
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
        if constexpr (CHasGetLocalRole<T>)
        {
            return Object.GetLocalRole();
        }
        else if constexpr (CHasGetOwnerRole<T>)
        {
            return Object.GetOwnerRole();
        }
        else
        {
            static_assert(always_false<T>::value,
                "T should have member GetLocalRole or GetOwnerRole that returns ENetRole");
            return {};
        }
    }
}

template <typename TCharType, typename T, bool bConstantStringLength = false>
void GetNetModeString(TStringBuilderBase<TCharType>& Builder, const T& Object)
{
    if (Rem::IsNetMode(Object, NM_DedicatedServer) || Rem::IsNetMode(Object, NM_ListenServer))
    {
        if constexpr (bConstantStringLength)
        {
            Builder << UTF8TEXTVIEW("Server  ");
        }
        else
        {
            Builder << UTF8TEXTVIEW("Server");
        }
    }
    else
    {
        Rem::Format(Builder, "Client:{}", UE::GetPlayInEditorID());
    }
}

template <typename T, bool bConstantStringLength = false>
FUtf8String GetNetModeString(const T& Object)
{
    TUtf8StringBuilder<16> Builder;
    Rem::GetNetModeString<UTF8CHAR, T, bConstantStringLength>(Builder, Object);
    return FUtf8String(Builder);
}

template <typename TCharType, typename T, bool bConstantStringLength = false>
void GetNetRoleString(TStringBuilderBase<TCharType>& Builder, const T& Object)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
    {
        Rem::GetNetRoleString(Builder, *Object);
    }
    else
    {
        Builder << UEnum::GetValueAsString(Rem::GetNetRole(Object));

        if constexpr (bConstantStringLength)
        {
            constexpr auto MaxLength = UTF8TEXTVIEW("ROLE_AutonomousProxy").Len();
            const auto CurrentLen    = Builder.Len();
            for (auto Counter = CurrentLen; Counter < MaxLength; ++Counter)
            {
                Builder.AppendChar(UTF8CHAR{' '});
            }
        }
    }
}

template <typename T, bool bConstantStringLength = false>
FUtf8String GetNetRoleString(const T& Object)
{
    TUtf8StringBuilder<32> Builder;
    Rem::GetNetRoleString<UTF8CHAR, T, bConstantStringLength>(Builder, Object);
    return FUtf8String(Builder);
}

template <typename T>
FName GetNetRoleName(const T& Object)
{
    return UEnum::GetValueAsName(Rem::GetNetRole(Object));
}

template <typename T>
FText GetNetRoleText(const T& Object)
{
    return UEnum::GetDisplayValueAsText(Rem::GetNetRole(Object));
}

template <typename TCharType, typename T>
void GetNetDebugString(TStringBuilderBase<TCharType>& Builder, const T& Object)
{
    if constexpr (std::is_pointer_v<T> || CNotNull<T>)
    {
        Rem::GetNetDebugString(Builder, *Object);
    }
    else
    {
        using RawType = std::remove_cvref_t<T>;

        if constexpr (TIsTObjectPtr<RawType>::Value)
        {
            Rem::GetNetDebugString(Builder, *Object);
        }
        else
        {
            Rem::GetNetModeString(Builder, Object);

            if constexpr (CHasGetLocalRole<T> || CHasGetOwnerRole<T>)
            {
                constexpr auto Space = UTF8CHAR{' '};
                Builder.AppendChar(Space);
                Rem::GetNetRoleString(Builder, Object);
            }
        }
    }
}

template <typename T>
FUtf8String GetNetDebugString(const T& Object)
{
    TUtf8StringBuilder<64> Builder;
    Rem::GetNetDebugString(Builder, Object);
    return FUtf8String(Builder);
}

template <CUObject T>
decltype(auto) GetDefaultRef()
{
    return *::GetDefault<T>();
}

template <CUObject T>
decltype(auto) GetMutableDefaultRef()
{
    return *::GetMutableDefault<T>();
}

constexpr uint8 RoleCount    = ROLE_MAX - 1u;
constexpr uint8 NetModeCount = NM_MAX;

REMCOMMON_API uint8* AllocateStructMemory(const UStruct& Struct);
REMCOMMON_API void FreeStructMemory(const UStruct& Struct, uint8& Memory);

REMCOMMON_API IConsoleVariable* FindConsoleVariable(const TCHAR* Name);

REMCOMMON_API float EvaluateCurve01(const FRealCurve& RichCurve, const float Alpha);
REMCOMMON_API float EvaluateCurve01Clamped(const FRealCurve& RichCurve, const float Alpha);
}

template <typename T, typename Char>
    requires (fmt::detail::type_constant<T, Char>::value == fmt::detail::type::custom_type
              && !std::is_void_v<std::remove_cvref_t<T>>)
struct fmt::formatter<T, Char> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const T& Value, FormatContext& Ctx) const
    {
        TUtf8StringBuilder<256> Builder;
        Rem::ToString(Builder, Value);
        return fmt::formatter<std::string_view>::format(
            std::string_view(reinterpret_cast<const ANSICHAR*>(Builder.GetData()), Builder.Len()), Ctx);
    }
};
