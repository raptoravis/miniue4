using UnrealBuildTool;
 
public class ExampleGameCore : ModuleRules
{
    public ExampleGameCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
     
        PublicDependencyModuleNames.AddRange(new string[] {  });
 
        PrivateDependencyModuleNames.AddRange(new string[] { "Core" });
    }
}