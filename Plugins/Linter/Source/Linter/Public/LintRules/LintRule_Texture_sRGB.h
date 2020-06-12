// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LintRule.h"
#include "Engine/Texture.h"
#include "LintRule_Texture_sRGB.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class ULintRule_Texture_sRGB : public ULintRule
{
	GENERATED_BODY()

public:
	ULintRule_Texture_sRGB(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSet<TEnumAsByte<TextureCompressionSettings>> DisableInTheseCompressions;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TSet<TEnumAsByte<TextureGroup>> DisableInTheseGroups;

	virtual bool PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;

protected:
	virtual bool PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const override;
};