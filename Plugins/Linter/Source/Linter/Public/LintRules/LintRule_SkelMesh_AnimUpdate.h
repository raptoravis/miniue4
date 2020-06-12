// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRule.h"
#include "LintRule_SkelMesh_AnimUpdate.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class LINTER_API ULintRule_SkelMesh_AnimUpdate : public ULintRule
{
	GENERATED_BODY()
	
public:
	ULintRule_SkelMesh_AnimUpdate(const FObjectInitializer& ObjectInitializer);

	virtual bool PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;

protected:
	virtual bool PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;

	bool CheckAnimUpdate(class USkeletalMeshComponent* SMC) const;
};
