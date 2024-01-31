// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "Engine/AssetManager.h"
#include "Macro/RemAssertionMacros.h"

struct FStreamableHandle;

namespace Rem
{
	template<Concepts::has_to_soft_object_path SoftType>
	TSharedPtr<FStreamableHandle> LoadAssets(const TConstArrayView<SoftType> SoftObjects,
		const FStreamableDelegate DelegateToCall = {},
		const TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		const FString& DebugName = FStreamableHandle::HandleDebugName_AssetList)
	{
		RemCheckCondition(SoftObjects.Num() > 0, return {});
		
		TArray<FSoftObjectPath> AssetList;
		{
			AssetList.Reserve(SoftObjects.Num());

			for (const SoftType& SoftObject : SoftObjects)
			{
				RemCheckCondition(!SoftObject.IsNull(), continue;);
				
				AssetList.Add(SoftObject.ToSoftObjectPath());
			}
		}

		return UAssetManager::Get().LoadAssetList(std::move(AssetList), DelegateToCall, Priority, DebugName);
	}

	template<Concepts::has_to_soft_object_path SoftType>
	TSharedPtr<FStreamableHandle> LoadAsset(const SoftType& SoftObject,
		const FStreamableDelegate DelegateToCall = {},
		const TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		const FString& DebugName = FStreamableHandle::HandleDebugName_AssetList)
	{
		return LoadAssets<SoftType>({SoftObject}, DelegateToCall, Priority, DebugName);
	}
}
