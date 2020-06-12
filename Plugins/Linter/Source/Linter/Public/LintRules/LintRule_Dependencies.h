// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRule.h"
#include "LintRule_Dependencies.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ULintRule_Dependencies : public ULintRule
{
	GENERATED_BODY()

public:
	ULintRule_Dependencies(const FObjectInitializer& ObjectInitializer);
	
	virtual bool PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;

protected:
	virtual bool PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;
};
