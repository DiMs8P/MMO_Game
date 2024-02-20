// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MMO_Client : ModuleRules
{
	public MMO_Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Json",
			"JsonUtilities",
			"Http"
		});
		
		PublicIncludePaths.AddRange(new string[] {"MMO_Client/Public/Player"});
	}
}
