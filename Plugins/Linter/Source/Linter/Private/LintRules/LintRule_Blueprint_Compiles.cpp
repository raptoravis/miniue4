// Copyright 2019-2020 Gamemakin LLC. All Rights Reserved.
#include "LintRules/LintRule_Blueprint_Compiles.h"
#include "LintRuleSet.h"
#include "Sound/SoundWave.h"
#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/CompilerResultsLog.h"
#include "BlueprintEditorSettings.h"

ULintRule_Blueprint_Compiles::ULintRule_Blueprint_Compiles(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bRequiresGameThread = true;
}

bool ULintRule_Blueprint_Compiles::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;
	UBlueprint* Blueprint = CastChecked<UBlueprint>(ObjectToLint);

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

	return bRuleViolated;
}