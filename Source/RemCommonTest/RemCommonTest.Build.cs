// Copyright RemRemRemRe. 2026. All Rights Reserved.

using UnrealBuildTool;
using Rem.BuildRule;

public class RemCommonTest : ModuleRules
{
	public RemCommonTest(ReadOnlyTargetRules target) : base(target)
	{
		RemSharedModuleRules.Apply(this);

		PrivateDependencyModuleNames.AddRange(
			[
				"Core",
				"CoreUObject",
				"Engine",
				"RenderCore",

				"RemCommon",
			]
		);
	}
}
