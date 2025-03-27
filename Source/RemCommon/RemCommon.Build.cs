// Copyright RemRemRemRe. 2025. All Rights Reserved.

using UnrealBuildTool;

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
			[
				"Core",
				"CoreUObject",
				"Engine",

				"DeveloperSettings",
				"GameplayTags"
			]
		);
	}
}
