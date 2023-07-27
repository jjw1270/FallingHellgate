// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FallingHellgate : ModuleRules
{
	public FallingHellgate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        PublicIncludePaths.AddRange(
            new string[] {
                "FallingHellgate",
                "FallingHellgate/Public",
                "FallingHellgate/Public/Framework",
                "FallingHellgate/Public/Characters",
                "FallingHellgate/Public/Characters/PlayableCharacter"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "FallingHellgate/Private",
                "FallingHellgate/Private/Framework",
                "FallingHellgate/Private/Characters",
                "FallingHellgate/Private/Characters/PlayableCharacter"
            }
        );
    }
}
