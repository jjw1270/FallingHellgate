// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class FallingHellgate : ModuleRules
{
	public FallingHellgate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "Niagara", "OnlineSubsystem", "OnlineSubsystemUtils" });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Niagara"));

        PublicIncludePaths.AddRange(
            new string[] {
                ModuleDirectory,
                Path.Combine(ModuleDirectory, "Public"),
                Path.Combine(ModuleDirectory, "Public/Framework"),
                Path.Combine(ModuleDirectory, "Public/Data"),
                Path.Combine(ModuleDirectory, "Public/Player"),
                Path.Combine(ModuleDirectory, "Public/Player/InventorySystem"),
                Path.Combine(ModuleDirectory, "Public/Player/QuickSlotSystem"),
                Path.Combine(ModuleDirectory, "Public/Player/EquipmentSystem"),
                Path.Combine(ModuleDirectory, "Public/Player/Notifies"),
                Path.Combine(ModuleDirectory, "Public/Player/Interfaces"),
                Path.Combine(ModuleDirectory, "Public/Player/AttackSystem"),
                Path.Combine(ModuleDirectory, "Public/Player/Status"),
                Path.Combine(ModuleDirectory, "Public/NPC"),
                Path.Combine(ModuleDirectory, "Public/NPC/Shop"),
                Path.Combine(ModuleDirectory, "Public/Title"),

            }
        );

    }
}
