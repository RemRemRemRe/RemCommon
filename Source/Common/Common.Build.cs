//

using UnrealBuildTool;

public class Common : ModuleRules
{
	public Common(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage					= PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths	= false;
		ShadowVariableWarningLevel	= WarningLevel.Error;

		PrivateDependencyModuleNames.AddRange(
			new [] {
				"Core",
				"CoreUObject",
				"Engine",
			}
		);
	}
}
