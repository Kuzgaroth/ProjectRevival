// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectRevival : ModuleRules
{
	public ProjectRevival(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			"ProjectRevival/Public/Player",
			"ProjectRevival/Public/Components",
			"ProjectRevival/Public/Weapon",
			"ProjectRevival/Public/UI",
			"ProjectRevival/Public/Miscellaneous",
			"ProjectRevival/Public/Pickup",
			"ProjectRevival/Public/Weapon/Components",
			"ProjectRevival/Public/Weapon/Projectile",
			"ProjectRevival/Public/Miscellaneous/AnimNotify",
			"ProjectRevival/Public/AI",
			"ProjectRevival/Public/AI/Tasks"
		});
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
