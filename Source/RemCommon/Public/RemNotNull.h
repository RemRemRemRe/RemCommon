// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Misc/NotNull.h"
#include "Templates/RemIsInstance.h"
#include "Macro/RemFunctorMacro.h"

namespace Rem
{
    template <typename T>
    using TNotNull = ::TNotNull<T>;
    
    template<typename T>
    decltype(auto) MakeNotNull(T&& Object)
    {
        using Type = std::remove_reference_t<T>;
        if constexpr (std::is_pointer_v<Type>)
        {
            return TNotNull<Type>{ Object };
        }
        else if constexpr (TIsTObjectPtr_V<Type>)
        {
            return TNotNull<typename Type::ElementType*>{ Object.Get() };
        }
        else
        {
            return TNotNull<Type*>{ &Object };
        }
    }
    
    REM_FUNCTION_TO_FUNCTOR_SIMPLE(MakeNotNull)
}
