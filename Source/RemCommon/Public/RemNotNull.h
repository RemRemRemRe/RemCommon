// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Misc/NotNull.h"
#include "Macro/RemFunctorMacro.h"

namespace Rem
{
    template <typename T>
    using TNotNull = ::TNotNull<T>;
    
    /**
     * pointer can convert / construct TNotNull implicitly, but there are cases to copy const from T
     */
    template<typename T>
    decltype(auto) MakeNotNull(T* Object)
    {
        return TNotNull<T*>{ Object };
    }
    
    REM_FUNCTION_TO_FUNCTOR_SIMPLE(MakeNotNull)
}
