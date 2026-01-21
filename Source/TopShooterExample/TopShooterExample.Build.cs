// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TopShooterExample : ModuleRules
{
	public TopShooterExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"GameplayCameras"
		});
		PrivateDependencyModuleNames.AddRange(new string[] { 	"Slate" , "SlateCore" });

		PublicIncludePaths.AddRange(new string[] {
			"TopShooterExample",
			"TopShooterExample/Variant_Platforming",
			"TopShooterExample/Variant_Platforming/Animation",
			"TopShooterExample/Variant_Combat",
			"TopShooterExample/Variant_Combat/AI",
			"TopShooterExample/Variant_Combat/Animation",
			"TopShooterExample/Variant_Combat/Gameplay",
			"TopShooterExample/Variant_Combat/Interfaces",
			"TopShooterExample/Variant_Combat/UI",
			"TopShooterExample/Variant_SideScrolling",
			"TopShooterExample/Variant_SideScrolling/AI",
			"TopShooterExample/Variant_SideScrolling/Gameplay",
			"TopShooterExample/Variant_SideScrolling/Interfaces",
			"TopShooterExample/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
