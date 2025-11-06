// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemInstancedStructStaics.inl"
#include "Macro/RemAssertionMacros.h"
#include "StructUtils/InstancedStructContainer.h"

#include "RemInstancedStructContainer.generated.h"

[[nodiscard]] inline int32 GetNum(const FInstancedStructContainer& Container)
{
    return Container.Num();
}

#define REM_API REMCOMMON_API

/**
 * an InstancedStructContainer with optimized layout and a map for quick indexing
 *
 * @note currently, structs would be stored in the oder of their MinAlignment for more compact layout
 */
USTRUCT(BlueprintType)
struct FRemInstancedStructContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Rem")
    FInstancedStructContainer StructContainer{};
    
    UPROPERTY(EditAnywhere, Category = "Rem")
    TMap<uint32, int32> IndexMap{};
    
    using FKeyType = uint32;

    REM_API FStructView TryGetView(const FKeyType Key);
    REM_API FConstStructView TryGetView(const FKeyType Key) const;

    template <Rem::Concepts::has_static_struct T>
    TStructView<T> TryGetView(const FKeyType Key)
    {
        const auto View = TryGetView(Key);

        if (!View.IsValid() || !View.GetScriptStruct()->IsChildOf(T::StaticStruct()))
        {
            return TStructView<T>{};
        }
        
        return Rem::Struct::TryMakeView<T>(View);
    }

    template <Rem::Concepts::has_static_struct T>
    TConstStructView<T> TryGetView(const FKeyType Key) const
    {
        const auto View = TryGetView(Key);

        if (!View.IsValid() || !View.GetScriptStruct()->IsChildOf(T::StaticStruct()))
        {
            return TConstStructView<T>{};
        }
        
        return Rem::Struct::TryMakeView<T>(View);
    }
    
    template <Rem::Concepts::has_static_struct T>
    T* TryGetPtr(const FKeyType Key) const
    {
        const auto View = TryGetView(Key);
        auto* Ptr = View.GetPtr<const T>();
        return const_cast<T*>(Ptr);
    }

    template <Rem::Concepts::has_static_struct T>
    T& Get(const FKeyType Key) const
    {
        const auto View = TryGetView(Key);
        auto& Ref = View.Get<const T>();
        return const_cast<T&>(Ref);
    }

    REM_API bool Contains(FKeyType Key) const;
    
    /**
     * add only if it didn't exist before
     */
    REM_API bool TryAdd(FKeyType Key, FConstStructView NewStruct);
    
    REM_API void Reset();
    REM_API void Empty();
};

#undef REM_API
