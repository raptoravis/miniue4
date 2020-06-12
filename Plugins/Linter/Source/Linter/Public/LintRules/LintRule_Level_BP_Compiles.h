// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRules/LintRule_Level_Base.h"
#include "LintRule_Level_BP_Compiles.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ULintRule_Level_BP_Compiles : public ULintRule_Level_Base
{
	GENERATED_BODY()

public:
	ULintRule_Level_BP_Compiles(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;
};