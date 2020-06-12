// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Texture_sRGB.h"

ULintRule_Texture_sRGB::ULintRule_Texture_sRGB(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisableInTheseCompressions.Add(TextureCompressionSettings::TC_Alpha);
	DisableInTheseCompressions.Add(TextureCompressionSettings::TC_Normalmap);
	DisableInTheseCompressions.Add(TextureCompressionSettings::TC_Masks);
	DisableInTheseCompressions.Add(TextureCompressionSettings::TC_HDR);
	DisableInTheseCompressions.Add(TextureCompressionSettings::TC_HDR_Compressed);

	DisableInTheseGroups.Add(TextureGroup::TEXTUREGROUP_ColorLookupTable);
}

bool ULintRule_Texture_sRGB::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	// If we aren't a texture, abort
	if (Cast<UTexture2D>(ObjectToLint) == nullptr)
	{
		// @TODO: Bubble up some sort of configuration error?
		return true;
	}

	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_Texture_sRGB::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	const UTexture2D* Texture = CastChecked<UTexture2D>(ObjectToLint);
	bool bPreventSRGB = DisableInTheseCompressions.Contains(Texture->CompressionSettings) || DisableInTheseGroups.Contains(Texture->LODGroup);
	if (!((bPreventSRGB ? 1 : 0) ^ (Texture->SRGB ? 1 : 0)))
	{
		FText RecommendedAction = FText::FormatOrdered(NSLOCTEXT("Linter", "Texture_sRGB_Fail", "Recommended: {0}"), 
			Texture->SRGB ? FText::FromString(TEXT("UnChecked"))  : FText::FromString(TEXT("Checked")));
		OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), RecommendedAction));
		return false;
	}

	return true;
}
