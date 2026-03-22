// Copyright RemRemRemRe. 2026. All Rights Reserved.

using UnrealBuildTool;
using Rem.BuildRule;

public class RemCommon : ModuleRules
{
	public RemCommon(ReadOnlyTargetRules target) : base(target)
	{
        RemSharedModuleRules.Apply(this);

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

namespace Rem.BuildRule
{
    public static class RemSharedModuleRules
    {
        public static void Apply(ModuleRules target)
        {
            target.CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Error;
            target.IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
            target.DefaultBuildSettings = BuildSettingsVersion.Latest;
            target.CppStandard = CppStandardVersion.EngineDefault;
            target.CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;
            
            target.CppCompileWarningSettings.NonInlinedGenCppWarningLevel = WarningLevel.Warning;
            target.bUseUnity = false;
            
            target.bAllowUETypesInNamespaces = true;
        }
    }
}
