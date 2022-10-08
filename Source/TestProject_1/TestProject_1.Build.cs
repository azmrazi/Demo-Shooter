// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestProject_1 : ModuleRules
{
	public TestProject_1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
