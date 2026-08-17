// Copyright RemRemRemRe. 2025. All Rights Reserved.

#include "Struct/RemInstancedStructContainer.h"

#include "RemBinarySearch.h"
#include "Macro/RemAssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemInstancedStructContainer)

FStructView FRemInstancedStructContainer::TryGetView(const FKeyType Key)
{
    auto* Index = IndexMap.Find(Key);
    RemEnsureVariable(REM_NO_ASSERTION, Index, return {});

    return StructContainer[*Index];
}

FConstStructView FRemInstancedStructContainer::TryGetView(const FKeyType Key) const
{
    auto* Index = IndexMap.Find(Key);
    RemEnsureVariable(REM_NO_ASSERTION, Index, return {});

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
    RemEnsureVariable(REM_NO_ASSERTION, NewStruct.GetScriptStruct(), return false);

    const auto IndexToInsert = Rem::Algo::LowerBound(StructContainer,
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

        // Engine workaround (ue5-main regression): FInstancedStructContainer::InsertAt
        // shifts tail item offsets before calling ReserveBytes, whose
        // check(ValuesSize <= AllocatedSize) then fails on any middle insert that grows
        // the allocation. Rebuild the container in order instead: collect views (no
        // struct copies), append them into a fresh container, then move-assign. Each
        // item is copied exactly once, and the old items are destroyed only after the
        // new container is fully built.
        TArray<FConstStructView> Reordered;
        Reordered.Reserve(StructContainer.Num() + 1);
        for (int32 Index = 0; Index <= StructContainer.Num(); ++Index)
        {
            if (Index == IndexToInsert)
            {
                Reordered.Emplace(NewStruct);
            }
            if (Index < StructContainer.Num())
            {
                Reordered.Emplace(StructContainer[Index]);
            }
        }

        FInstancedStructContainer NewContainer;
        NewContainer.Append(Reordered);
        StructContainer = MoveTemp(NewContainer);
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
