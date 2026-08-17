// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "Containers/Map.h"

namespace Rem
{

template <typename Container>
    requires (TIsTMap_V<Container> && CHasIsValid<typename Container::KeyType>)
void RemoveInvalidKey(Container& Map)
{
    for (auto Iter{Map.CreateIterator()}; Iter; ++Iter)
    {
        if (!Iter->Key.IsValid())
        {
            Iter.RemoveCurrent();
        }
    }
}

}
