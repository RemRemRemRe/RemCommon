// Copyright RemRemRemRe. 2025. All Rights Reserved.

#pragma once

#include "RemConcepts.h"
#include "Engine/AssetManager.h"
#include "GameplayTag/RemGameplayTagStatics.h"
#include "Macro/RemAssertionMacros.h"

struct FStreamableHandle;

namespace Rem
{
	template<typename SoftType>
	requires Concepts::has_to_soft_object_path<SoftType> || std::is_base_of_v<FSoftObjectPath, SoftType>
	TSharedPtr<FStreamableHandle> LoadAssets(const TConstArrayView<SoftType> SoftObjects,
		const FStreamableDelegate DelegateToCall = {},
		const TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		FString DebugName = FStreamableHandle::HandleDebugName_AssetList)
	{
		RemCheckCondition(SoftObjects.Num() > 0, return {});

		if constexpr (std::is_base_of_v<FSoftObjectPath, SoftType>)
		{
			return UAssetManager::Get().LoadAssetList(TArray<FSoftObjectPath>{SoftObjects}, DelegateToCall, Priority, std::move(DebugName));
		}
		else
		{
			TArray<FSoftObjectPath> AssetList;
			{
				AssetList.Reserve(SoftObjects.Num());

				for (const SoftType& SoftObject : SoftObjects)
				{
					RemCheckCondition(!SoftObject.IsNull(), continue;);

					AssetList.Add(SoftObject.ToSoftObjectPath());
				}
			}
			return UAssetManager::Get().LoadAssetList(std::move(AssetList), DelegateToCall, Priority, std::move(DebugName));
		}
	}

	template<typename SoftType>
	requires Concepts::has_to_soft_object_path<SoftType> || std::is_base_of_v<FSoftObjectPath, SoftType>
	TSharedPtr<FStreamableHandle> LoadAsset(const SoftType& SoftObject,
		const FStreamableDelegate DelegateToCall = {},
		const TAsyncLoadPriority Priority = FStreamableManager::DefaultAsyncLoadPriority,
		FString DebugName = FStreamableHandle::HandleDebugName_AssetList)
	{
		return LoadAssets<SoftType>({SoftObject}, DelegateToCall, Priority, DebugName);
	}

	template<typename SoftType>
	requires Concepts::has_to_soft_object_path<SoftType> || std::is_base_of_v<FSoftObjectPath, SoftType>
	bool IsLoaded(const SoftType& SoftObject)
	{
		if constexpr (std::is_base_of_v<FSoftObjectPath, SoftType>)
		{
			return !!SoftObject.ResolveObject();
		}
		else
		{
			return !!SoftObject.ToSoftObjectPath().ResolveObject();
		}
	}

	template<typename SoftType>
	requires Concepts::has_to_soft_object_path<SoftType> || std::is_base_of_v<FSoftObjectPath, SoftType>
	bool IsLoaded(const TConstArrayView<SoftType> SoftObjects)
	{
		for (auto& Path : SoftObjects)
		{
			if (!IsLoaded(Path))
			{
				return false;
			}
		}

		return true;
	}

	template<Concepts::is_primary_data_asset T>
	const T* GetPrimaryAsset(const FPrimaryAssetId& AssetId)
	{
		const auto& AssetPath = UAssetManager::Get().GetPrimaryAssetPath(AssetId);
		const auto* AssetLoaded = Cast<T>(AssetPath.ResolveObject());
		return AssetLoaded;
	}

	template<Concepts::is_primary_data_asset T>
	const T* GetPrimaryAsset(const FGameplayTag& AssetTag)
	{
		const auto AssetId = GameplayTag::MakePrimaryAssetIdFromTag(AssetTag);
		return GetPrimaryAsset<T>(AssetId);
	}
}
