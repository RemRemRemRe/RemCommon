//


namespace UnrealBuildTool.Rules
{
	public class RemCommon : ModuleRules
	{
		public RemCommon(ReadOnlyTargetRules target) : base(target)
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