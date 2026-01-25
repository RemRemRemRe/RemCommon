// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Macro/RemFunctorMacro.h"

#include <type_traits>

namespace Rem
{
    template <typename T>
    auto RefToPtr(std::reference_wrapper<T> ReferenceWrapper) -> decltype(auto)
    {
        return &ReferenceWrapper.get();
    }
    REM_FUNCTION_TO_FUNCTOR_SIMPLE(RefToPtr)

}
