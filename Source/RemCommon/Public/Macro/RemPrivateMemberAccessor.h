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

    // Returns the value of the data member or invokes the member function referenced by this accessor.
    template <typename AccessorName, typename ThisType, typename... ArgumentsType>
    decltype(auto) Access(ThisType&& This, ArgumentsType&&... Arguments)
    {
        if constexpr (std::is_pointer_v<std::remove_reference_t<ThisType>>)
        {
            if constexpr (sizeof...(ArgumentsType) > 0)
            {
                return (Forward<ThisType>(This)->*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
            }
            else
            {
                return Forward<ThisType>(This)->*TMemberPointer<AccessorName>;
            }
        }
        else
        {
            if constexpr (sizeof...(ArgumentsType) > 0)
            {
                return (Forward<ThisType>(This).*TMemberPointer<AccessorName>)(Forward<ArgumentsType>(Arguments)...);
            }
            else
            {
                return Forward<ThisType>(This).*TMemberPointer<AccessorName>;
            }
        }
    }
}

/**
 * Alternative to UE_DEFINE_PRIVATE_MEMBER_PTR() that works with overloaded functions.
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
	template struct Rem::PrivateMemberAccessor::TPointerInitializer<AccessorName, MemberPointer>; \
