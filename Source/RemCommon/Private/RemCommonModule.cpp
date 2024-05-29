// Copyright RemRemRemRe, All Rights Reserved.

#include "RemCommonModule.h"
#include "Modules/ModuleManager.h"

#include "RemConcepts.h"

#if WITH_EDITOR

#include "GameplayTagsManager.h"
#include "GameplayTag/RemGameplayTagWithCategory.h"

#include "PropertyHandle.h"

#endif

IRemCommonModule& IRemCommonModule::Get()
{
	return FModuleManager::LoadModuleChecked< IRemCommonModule >( "RemCommon" );
}

bool IRemCommonModule::IsAvailable()
{
	return FModuleManager::Get().IsModuleLoaded( "RemCommon" );
}


class FRemCommonModule : public IRemCommonModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	using ThisClass = FRemCommonModule;
	
#if WITH_EDITOR
	
	FDelegateHandle DelegateHandle;
	static void OnGetCategoriesMetaFromPropertyHandle(const TSharedPtr<IPropertyHandle> PropertyHandle, FString& OutCategoryString);

#endif
};

IMPLEMENT_MODULE(FRemCommonModule, RemCommon);

void FRemCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	IRemCommonModule::StartupModule();

#if WITH_EDITOR
	
	DelegateHandle = UGameplayTagsManager::Get().OnGetCategoriesMetaFromPropertyHandle.AddStatic(
		&ThisClass::OnGetCategoriesMetaFromPropertyHandle);

#endif
}

void FRemCommonModule::ShutdownModule()
{
#if WITH_EDITOR
	
	if (UGameplayTagsManager* GameplayTagsManager = UGameplayTagsManager::GetIfAllocated())
	{
		GameplayTagsManager->OnGetCategoriesMetaFromPropertyHandle.Remove(DelegateHandle);
	}

#endif

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IRemCommonModule::ShutdownModule();
}

#if WITH_EDITOR

// ReSharper disable once CppPassValueParameterByConstReference
void FRemCommonModule::OnGetCategoriesMetaFromPropertyHandle(const TSharedPtr<IPropertyHandle> PropertyHandle,
	FString& OutCategoryString)
{
	static_assert(Rem::Concepts::has_tag<FRemGameplayTagWithCategory>, "Tag member of FRemGameplayTagWithCategory is missing!");
	const auto TagPropertyName = FName{TEXTVIEW("Tag")};
	
	if (const auto* Property = PropertyHandle->GetProperty();
		Property->GetFName() == TagPropertyName && Property->GetOwnerStruct() == FRemGameplayTagWithCategory::StaticStruct())
	{
		if (void* OutAddress = nullptr;
			FPropertyAccess::Success == PropertyHandle->GetParentHandle()->GetValueData(OutAddress))
		{
			if (const auto* GameplayTagWithCategory = static_cast<const FRemGameplayTagWithCategory*>(OutAddress);
				GameplayTagWithCategory && GameplayTagWithCategory->GetCategory().IsValid())
			{
				OutCategoryString = GameplayTagWithCategory->GetCategory().GetTagName().ToString();
			}
		}
	}
}

#endif