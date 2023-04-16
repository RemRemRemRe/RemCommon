//


namespace UnrealBuildTool.Rules
{
	public class Common : ModuleRules
	{
		public Common(ReadOnlyTargetRules target) : base(target)
		{
			PCHUsage					= PCHUsageMode.UseExplicitOrSharedPCHs;
			bLegacyPublicIncludePaths	= false;
			ShadowVariableWarningLevel	= WarningLevel.Error;
			CppStandard 				= CppStandardVersion.Cpp20;

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