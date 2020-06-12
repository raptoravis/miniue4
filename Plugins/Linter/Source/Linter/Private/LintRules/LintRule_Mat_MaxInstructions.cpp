// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Mat_MaxInstructions.h"
#include "RHIDefinitions.h"
#include "Materials/Material.h"
#include "MaterialStatsCommon.h"
#include "CoreGlobals.h"
#include "SuperUtilsBPLibrary.h"

ULintRule_Mat_MaxInstructions::ULintRule_Mat_MaxInstructions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bRequiresGameThread = true;
	MaxInstructions = 200;
}

bool ULintRule_Mat_MaxInstructions::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	if (!IsRunningCommandlet())
		return true;
	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_Mat_MaxInstructions::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;

	UMaterial* Material = CastChecked<UMaterial>(ObjectToLint);
	TSet<ERHIFeatureLevel::Type> FeatureLevels = { ERHIFeatureLevel::ES3_1 };
	TMap<ERHIFeatureLevel::Type, TArray<FMaterialStatsUtils::FShaderInstructionsInfo>> Results;
	USuperUtilsBPLibrary::GetMaterialInstructionCounts(FeatureLevels, Material, Results);

	TArray<FText> Messages;
	FText RecommendedAction = FText::FormatOrdered(NSLOCTEXT("Linter", "Materail Max Instructions", "Recommended: Max allowed Instructions is {0}."), FText::AsNumber(MaxInstructions));
	Messages.Add(RecommendedAction);

	for (auto& Result : Results)
	{
		TArray<FMaterialStatsUtils::FShaderInstructionsInfo>& ShaderInstructionsInfos = Result.Value;
		for (auto ShaderInstructionsInfo : ShaderInstructionsInfos)
		{
			if (ShaderInstructionsInfo.InstructionCount > MaxInstructions)
			{
				Messages.Add(
					FText::FormatOrdered(NSLOCTEXT("Linter", "Materail Instructions", "{0}, {1} Instructions."),
						FText::FromString(ShaderInstructionsInfo.ShaderDescription),
						FText::AsNumber(ShaderInstructionsInfo.InstructionCount))
				);
				bRuleViolated = false;
			}
		}
	}

	if (!bRuleViolated)
	{
		FText ResultMessage = FText::Join(FText::FromString(LINE_TERMINATOR), Messages);
		OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), ResultMessage));
	}

	return bRuleViolated;
}

