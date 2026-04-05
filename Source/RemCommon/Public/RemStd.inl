// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include <tuple>
#include <utility>
#include <type_traits>

namespace RemStd
{
    template <typename F, typename... BoundArgs>
    constexpr auto bind_back(F&& f, BoundArgs&&... bound_args)
    {
        // perfect forwarding every argument
        auto tuple = std::make_tuple(std::forward<BoundArgs>(bound_args)...);
    
        // return the callable
        return [func = std::forward<F>(f), bound_tuple = std::move(tuple)]
        (auto&&... call_args) -> decltype(auto)
            {
                // passing bound_tuple to the func to invoke with apply
                return std::apply(
                    [&](auto&&... bound) -> decltype(auto)
                        {
                            // forward every argument 
                            return std::invoke(
                                func,
                                std::forward<decltype(call_args)>(call_args)...,
                                std::forward<decltype(bound)>(bound)...);
                        },
                    bound_tuple);
            };
    }
}
