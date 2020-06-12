// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Level_BP_Compiles.h"
#include "Engine/Level.h"
#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/CompilerResultsLog.h"
#include "BlueprintEditorSettings.h"
#include "Engine/World.h"

ULintRule_Level_BP_Compiles::ULintRule_Level_BP_Compiles(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bRequiresGameThread = true;
}

bool ULintRule_Level_BP_Compiles::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;
	UWorld* World = Cast<UWorld>(ObjectToLint);
	if (ULevel* Level = World->PersistentLevel)
	{
		for (UBlueprint* Blueprint : Level->GetLevelBlueprints())
		{
			FCompilerResultsLog LogResults;
			LogResults.SetSourcePath(Blueprint->GetPathName());
			LogResults.BeginEvent(TEXT("Compile"));
			LogResults.bLogDetailedResults = GetDefault<UBlueprintEditorSettings>()->bShowDetailedCompileResults;
			LogResults.EventDisplayThresholdMs = GetDefault<UBlueprintEditorSettings>()->CompileEventDisplayThresholdMs;
			EBlueprintCompileOptions CompileOptions = EBlueprintCompileOptions::SkipSave | EBlueprintCompileOptions::SkipGarbageCollection;
			FKismetEditorUtilities::CompileBlueprint(Blueprint, CompileOptions, &LogResults);
			if (LogResults.NumWarnings > 0 || LogResults.NumErrors > 0)
			{
				FText RecommendedAction = NSLOCTEXT("Linter", "BlueprintCompiles", "Compile Results:");
				TArray<FText> Messages;
				Messages.Add(RecommendedAction);
				for (auto& Message : LogResults.Messages)
					Messages.Add(Message->ToText());
				FText ResultMessage = FText::Join(FText::FromString(TEXT("\n")), Messages);
				OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), ResultMessage));
				bRuleViolated = false;
			}
			LogResults.EndEvent();
		}
	}

	return bRuleViolated;
}
