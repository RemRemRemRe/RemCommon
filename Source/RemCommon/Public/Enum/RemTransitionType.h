// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "RemEnumStatics.inl"

#include "RemTransitionType.generated.h"

namespace Rem::Enum
{
    enum class ETransitionType : uint8;

    UENUM(BlueprintType)
    enum class ERemTransitionType : uint8
    {
        Abort,
        ToNext,
        ToPrevious,
        ToStart,
        ToEnd,
        Restart,
    };
    
    template<ERemTransitionType Transition, typename T>
    requires std::is_signed_v<T>
    [[nodiscard]] constexpr const TTuple<ETransitionType, T>& EnumToTransition()
    {
        if constexpr (Transition == ERemTransitionType::Abort)
        {
            return Transition::Abort<T>;
        }
        else if constexpr (Transition == ERemTransitionType::ToNext)
        {
            return Transition::ToNext<T>;
        }
        else if constexpr (Transition == ERemTransitionType::ToPrevious)
        {
            return Transition::ToPrevious<T>;
        }
        else if constexpr (Transition == ERemTransitionType::ToStart)
        {
            return Transition::ToStart<T>;
        }
        else if constexpr (Transition == ERemTransitionType::ToEnd)
        {
            return Transition::ToEnd<T>;
        }
        else if constexpr (Transition == ERemTransitionType::Restart)
        {
            return Transition::Restart<T>;
        }
        else
        {
            static_assert(Rem::always_false<T>::value, "Unsupported Transition Type");
            return Transition::Abort<T>;
        }
    }
    
    template<typename T>
    requires std::is_signed_v<T>
    [[nodiscard]] constexpr const TTuple<ETransitionType, T>& EnumToTransition(const ERemTransitionType Transition)
    {
        if (Transition == ERemTransitionType::Abort)
        {
            return EnumToTransition<ERemTransitionType::Abort, T>();
        }
        if (Transition == ERemTransitionType::ToNext)
        {
            return EnumToTransition<ERemTransitionType::ToNext, T>();
        }
        if (Transition == ERemTransitionType::ToPrevious)
        {
            return EnumToTransition<ERemTransitionType::ToPrevious, T>();
        }
        if (Transition == ERemTransitionType::ToStart)
        {
            return EnumToTransition<ERemTransitionType::ToStart, T>();
        }
        if (Transition == ERemTransitionType::ToEnd)
        {
            return EnumToTransition<ERemTransitionType::ToEnd, T>();
        }
        // if (Transition == ERemTransitionType::Restart)
        {
#if REM_WITH_DEVELOPMENT_ONLY_CODE
            
            RemCheckCondition(Transition == ERemTransitionType::Restart);
            
#endif
            
            return EnumToTransition<ERemTransitionType::Restart, T>();
        }
    }
}
