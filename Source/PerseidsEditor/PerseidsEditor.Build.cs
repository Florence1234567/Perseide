using UnrealBuildTool;

public class PerseidsEditor : ModuleRules
{
    public PerseidsEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Perseids"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Slate",
                "SlateCore",
                "UnrealEd",
                "AssetDefinition",
                "GraphEditor",
                "ToolMenus",
                // "ApplicationCore",
                // "InputCore"
            }
        );
    }
}