// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

// Container overloads for type-directed lookup. Include this file (in addition to
// RemScopedStructContainer.h) when you need FindStructView / FindConstStructView
// on a TScopedStructContainer; the container header itself stays dependency-light.

#include "Struct/RemScopedStructContainer.h"
#include "Struct/RemStructViewStatics.inl"

namespace Rem::Struct
{
/**
 * Type lookup over the container elements: returns the first element whose script
 * struct is TStructType or a child of it, paired with its index (INDEX_NONE when
 * absent). Same IsChildOf match semantics as the TArrayView overloads in
 * RemInstancedStructStatics.inl; the returned view points into container memory,
 * so writes are visible to the caller. Const container yields a read-only view.
 */
template <CHasStaticStruct TStructType, int32 InlineBytes, int32 OverflowChunkBytes>
[[nodiscard]] auto FindStructView(TScopedStructContainer<InlineBytes, OverflowChunkBytes>& Container)
{
    return Private::FindStructViewInternal<false, TStructType>(Container);
}

template <CHasStaticStruct TStructType, int32 InlineBytes, int32 OverflowChunkBytes>
[[nodiscard]] auto FindConstStructView(const TScopedStructContainer<InlineBytes, OverflowChunkBytes>& Container)
{
    return Private::FindStructViewInternal<true, TStructType>(Container);
}
}
