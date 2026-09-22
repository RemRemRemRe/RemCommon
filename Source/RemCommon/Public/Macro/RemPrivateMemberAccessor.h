// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <type_traits>

// import from https://github.com/Sixze/ALS-Refactored/blob/main/Source/ALS/Public/Utility/AlsPrivateMemberAccessor.h

namespace Rem::PrivateMemberAccessor
{
// Global pointer to a field or method.
template <typename AccessorName>
AccessorName::MemberType TMemberPointer;

// Struct that is used to initialize TPointer.
template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
struct TPointerInitializer
{
    TPointerInitializer()
    {
        TMemberPointer<AccessorName> = MemberPointer;
    }

    static TPointerInitializer Instance;
};

// Declaration of a TPointerInitializer instance.
template <typename AccessorName, typename AccessorName::MemberType MemberPointer>
TPointerInitializer<AccessorName, MemberPointer> TPointerInitializer<AccessorName, MemberPointer>::Instance;

// Returns the value of the data member, invokes the member function, or dereferences the
// static member referenced by this accessor. The receiver is ignored for static members.
template <typename AccessorName, typename ThisType, typename... ArgumentsType>
decltype(auto) Access(ThisType&& This, ArgumentsType&&... Arguments)
{
    using MemberType = typename AccessorName::MemberType;

    if constexpr (std::is_member_pointer_v<MemberType>)
    {
        // Instance member: requires an object (or object pointer) receiver.
        if constexpr (std::is_member_function_pointer_v<MemberType>)
        {
            if constexpr (std::is_pointer_v<std::remove_reference_t<ThisType>>)
            {
                return (Forward<ThisType>(This)->*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
            }
            else
            {
                return (Forward<ThisType>(This).*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
            }
        }
        else
        {
            static_assert(sizeof...(ArgumentsType) == 0,
                "REM_DEFINE_PRIVATE_MEMBER_ACCESSOR: data members cannot be invoked with arguments");
            if constexpr (std::is_pointer_v<std::remove_reference_t<ThisType>>)
            {
                return Forward<ThisType>(This)->*TMemberPointer<AccessorName>;
            }
            else
            {
                return Forward<ThisType>(This).*TMemberPointer<AccessorName>;
            }
        }
    }
    else
    {
        // Static member: MemberType is a plain pointer (data) or a function pointer.
        using PointeeType = std::remove_pointer_t<MemberType>;
        if constexpr (std::is_function_v<PointeeType>)
        {
            // Invoke the static function, with or without arguments.
            return TMemberPointer<AccessorName>(Forward<ArgumentsType>(Arguments)...);
        }
        else
        {
            static_assert(sizeof...(ArgumentsType) == 0,
                "REM_DEFINE_PRIVATE_MEMBER_ACCESSOR: static data members cannot be invoked with arguments");
            return *TMemberPointer<AccessorName>;
        }
    }
}
}

/**
 * Alternative to UE_DEFINE_PRIVATE_MEMBER_PTR() that works with overloaded functions,
 * zero-argument member functions, and static members.
 *
 * Examples:
 * 
 * REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(GGameplayTagCountContainerAccessor, &UAbilitySystemComponent::GameplayTagCountContainer,
    FGameplayTagCountContainer UAbilitySystemComponent::*);
 *
 * GGameplayTagCountContainerAccessor::Access(AbilitySystem);
 * 
 * REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(GOnTagUpdatedAccessor, &UAbilitySystemComponent::OnTagUpdated,
    void (UAbilitySystemComponent::*)(const FGameplayTag& Tag, bool bTagExists));
 *
 * GOnTagUpdatedAccessor::Access(AbilitySystem, Tag, bTagExists);
 *
 * REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(GStaticValueAccessor, &FExample::StaticValue, int32*);
 *
 * GStaticValueAccessor::Access(Example); // the receiver is ignored for static members
 *
 * REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(GStaticFunctionAccessor, &FExample::StaticFunction, int32 (*)(int32));
 *
 * GStaticFunctionAccessor::Access(Example, 1); // the receiver is ignored for static members
 *
 */
#define REM_DEFINE_PRIVATE_MEMBER_ACCESSOR(AccessorName, MemberPointer, ...) \
    namespace \
    { \
	    struct AccessorName \
	    { \
            using MemberType = __VA_ARGS__; \
            \
            template <typename ThisType, typename... ArgumentsType> \
            static decltype(auto) Access(ThisType&& This, ArgumentsType&&... Arguments) \
            { \
                return Rem::PrivateMemberAccessor::Access<AccessorName>(Forward<ThisType>(This), Forward<ArgumentsType>(Arguments)...); \
            } \
	    }; \
    } \
	\
	template struct Rem::PrivateMemberAccessor::TPointerInitializer<AccessorName, MemberPointer>;
