//


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
			CppStandard = CppStandardVersion.Cpp20;

			bEnableNonInlinedGenCppWarnings = true;

			PrivateDependencyModuleNames.AddRange(
				new [] {
					"Core",
					"CoreUObject",
					"Engine",
				}
			);
		}
	}
}