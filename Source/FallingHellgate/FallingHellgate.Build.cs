// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class FallingHellgate : ModuleRules
{
	public FallingHellgate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
                ModuleDirectory,
                Path.Combine(ModuleDirectory, "Public"),
                Path.Combine(ModuleDirectory, "Public/Framework"),
                Path.Combine(ModuleDirectory, "Public/InventorySystem"),
                Path.Combine(ModuleDirectory, "Public/QuickSlotSystem"),
                Path.Combine(ModuleDirectory, "Public/EquipmentSystem"),
                Path.Combine(ModuleDirectory, "Public/Characters"),
                Path.Combine(ModuleDirectory, "Public/Data"),
                Path.Combine(ModuleDirectory, "Public/Notifies"),
                Path.Combine(ModuleDirectory, "Public/Interfaces"),

            }
        );
    }
}
