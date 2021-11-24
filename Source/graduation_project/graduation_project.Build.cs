// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class graduation_project : ModuleRules
{
	public graduation_project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Niagara", "AIModule" });
		PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Json", "JsonUtilities", "GameplayTasks", "Paper2D" });
	}
}
