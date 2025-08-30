// Copyright RemRemRemRe. 2025. All Rights Reserved.

using UnrealBuildTool;

public class RemCommon : ModuleRules
{
	public RemCommon(ReadOnlyTargetRules target) : base(target)
	{
		CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Error;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		CppStandard = CppStandardVersion.EngineDefault;
		CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;

		CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;
		bUseUnity = false;

		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",

				"DeveloperSettings",
				"GameplayTags",
                
				"strong_alias",
			]
		);
	}
}
