using UnrealBuildTool;

public class YourGameTests : ModuleRules
{
    public YourGameTests(ReadOnlyTargetRules Target) : base(Target)
    {
                PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
                PrivateDependencyModuleNames.AddRange(new string[] {
                    "Core",
                    "Engine",
                    "CoreUObject",
                    "miniue4"
                });
    }
}
