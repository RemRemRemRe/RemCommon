// Copyright . All Rights Reserved.

#include "RemCommonModule.h"

class FRemCommonModule : public IRemCommonModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FRemCommonModule, RemCommon);

void FRemCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	IRemCommonModule::StartupModule();
}

void FRemCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	IRemCommonModule::ShutdownModule();
}