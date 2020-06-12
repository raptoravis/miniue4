// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRule.h"
#include "LintRule_Mat_Base.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ULintRule_Mat_Base : public ULintRule
{
	GENERATED_BODY()

public:
	ULintRule_Mat_Base(const FObjectInitializer& ObjectInitializer);

	virtual bool PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;
};
