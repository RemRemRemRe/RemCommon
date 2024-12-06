// Copyright RemRemRemRe, All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class RemCommon : ModuleRules
	{
		public RemCommon(ReadOnlyTargetRules target) : base(target)
		{
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
					"GameplayTags"
				}
			);
		}
	}
}
