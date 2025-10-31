// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "Struct/RemInstancedStructContainer.h"

#include "RemBinarySearch.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemInstancedStructContainer)

FStructView FRemInstancedStructContainer::TryGetView(const FKeyType Key)
{
    auto* Index = IndexMap.Find(Key);
    RemEnsureVariable(Index, return {}, REM_NO_LOG_OR_ASSERTION);
    
    return StructContainer[*Index];
}

FConstStructView FRemInstancedStructContainer::TryGetView(const FKeyType Key) const
{
    auto* Index = IndexMap.Find(Key);
    RemEnsureVariable(Index, return {}, REM_NO_LOG_OR_ASSERTION);
    
    return StructContainer[*Index];
}

bool FRemInstancedStructContainer::Contains(const FKeyType Key) const
{
    return IndexMap.Contains(Key);
}

bool FRemInstancedStructContainer::TryAdd(const FKeyType Key, const FConstStructView NewStruct)
{
    if (Contains(Key))
    {
        return false;
    }

    // StructMemory is optional
    RemEnsureVariable(NewStruct.GetScriptStruct(), return false, REM_NO_LOG_OR_ASSERTION);

    const auto IndexToInsert = Rem::Algo::UpperBound(StructContainer,
        NewStruct.GetScriptStruct()->GetMinAlignment(),
        [](const FConstStructView View)
        {                    
            return View.GetScriptStruct()->GetMinAlignment();
        });

    if (IndexToInsert == StructContainer.Num())
    {
        // it has the greatest alignment
        StructContainer.Append(MakeConstArrayView(&NewStruct, 1));
    }
    else
    {
        // first alignment greater than it is found

        // increment existed indexes
        for (auto& IndexPair : IndexMap)
        {
            if (IndexPair.Value >= IndexToInsert)
            {
                ++IndexPair.Value;
            }
        }

        // insert new one
        StructContainer.InsertAt(IndexToInsert, MakeConstArrayView(&NewStruct, 1));
    }
                
    IndexMap.Add(Key, IndexToInsert);

    return true;
}

void FRemInstancedStructContainer::Reset()
{
    StructContainer.Reset();
    IndexMap.Reset();
}

void FRemInstancedStructContainer::Empty()
{
    StructContainer.Empty();
    IndexMap.Empty();
}
