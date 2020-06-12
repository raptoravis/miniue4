// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Level_Base.h"
#include "Engine/Level.h"

ULintRule_Level_Base::ULintRule_Level_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULintRule_Level_Base::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	if (Cast<UWorld>(ObjectToLint) == nullptr)
	{
		return true;
	}

	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}
