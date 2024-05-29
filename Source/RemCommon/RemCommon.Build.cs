// Copyright RemRemRemRe, All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class RemCommon : ModuleRules
	{
		public RemCommon(ReadOnlyTargetRules target) : base(target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			ShadowVariableWarningLevel = WarningLevel.Error;
			IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
			DefaultBuildSettings = BuildSettingsVersion.Latest;
			CppStandard = CppStandardVersion.EngineDefault;
			UnsafeTypeCastWarningLevel = WarningLevel.Warning;

			bEnableNonInlinedGenCppWarnings = true;
			bUseUnity = false;

			PrivateDependencyModuleNames.AddRange(
				new [] {
					"Core",
					"CoreUObject",
					"Engine",
					
					"DeveloperSettings",
					"StructUtils",
					"GameplayTags"
				}
			);
			
			if (Target.bBuildEditor)
			{
				PrivateDependencyModuleNames.AddRange(
					new[] {
						"PropertyEditor",
					}
				);
			}
		}
	}
}