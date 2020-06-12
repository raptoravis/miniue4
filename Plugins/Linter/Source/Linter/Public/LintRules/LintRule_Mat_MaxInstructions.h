// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRule_Mat_Base.h"
#include "LintRule_Mat_MaxInstructions.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class LINTER_API ULintRule_Mat_MaxInstructions : public ULintRule_Mat_Base
{
	GENERATED_BODY()

public:
	ULintRule_Mat_MaxInstructions(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 MaxInstructions;

	virtual bool PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;

protected:
	virtual bool PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;
};
