// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#define REM_FUNCTION_TO_FUNCTOR(Name, Function) \
    namespace Private \
    { \
        struct Name##_Fn_Impl \
        { \
            template<typename... TArgs> \
            [[nodiscard]] constexpr auto operator()(TArgs&&... Args) const -> decltype(Function(std::forward<TArgs>(Args)...)) \
            { \
                return Function(std::forward<TArgs>(Args)...); \
            } \
        }; \
    }; \
    namespace Fn \
    { \
        constexpr Private::Name##_Fn_Impl Name{}; \
    }

#define REM_FUNCTION_TO_FUNCTOR_SIMPLE(Function) REM_FUNCTION_TO_FUNCTOR(Function, Function)
