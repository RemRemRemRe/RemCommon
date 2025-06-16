// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include <concepts>

class UStateTreeComponent;
class UAbilitySystemComponent;

namespace Rem::Concepts
{

template<class T>
concept is_ability_system_component = std::derived_from<T, UAbilitySystemComponent>;

template<class T>
concept is_statetree_component = std::derived_from<T, UStateTreeComponent>;

}
