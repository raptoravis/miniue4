// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
    public class miniue4Editor : ModuleRules
    {
        public miniue4Editor(ReadOnlyTargetRules Target) : base(Target)
        {
            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "UnrealEd",
                    "Slate",
                    "SlateCore",
                    "EditorStyle",
                    "GraphEditor",
                    "BlueprintGraph",
                    "KismetCompiler",
                    "MessageLog",
                    "miniue4"
                }
            );
        }
    }
}
