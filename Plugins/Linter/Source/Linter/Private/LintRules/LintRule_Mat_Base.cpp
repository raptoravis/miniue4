// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Mat_Base.h"
#include "Materials/Material.h"

ULintRule_Mat_Base::ULintRule_Mat_Base(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULintRule_Mat_Base::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	UMaterial* Material = Cast<UMaterial>(ObjectToLint);
	if (!Material)
		return true;
	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}
