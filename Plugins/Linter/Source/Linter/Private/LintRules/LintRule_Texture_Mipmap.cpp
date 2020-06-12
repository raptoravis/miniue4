// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Texture_Mipmap.h"

ULintRule_Texture_Mipmap::ULintRule_Texture_Mipmap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisableInTheseGroups.Add(TextureGroup::TEXTUREGROUP_UI);
	DisableInTheseGroups.Add(TextureGroup::TEXTUREGROUP_Pixels2D);
}

bool ULintRule_Texture_Mipmap::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	// If we aren't a texture, abort
	if (Cast<UTexture2D>(ObjectToLint) == nullptr)
	{
		// @TODO: Bubble up some sort of configuration error?
		return true;
	}

	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_Texture_Mipmap::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	const UTexture2D* Texture = CastChecked<UTexture2D>(ObjectToLint);
	bool bPreventMipMaps = DisableInTheseGroups.Contains(Texture->LODGroup);
	if (Texture->MipGenSettings != TMGS_FromTextureGroup)
	{
		if ((bPreventMipMaps ? 1 : 0) ^ ((Texture->MipGenSettings == TMGS_NoMipmaps) ? 1 : 0))
		{
			FText RecommendedAction = FText::FormatOrdered(NSLOCTEXT("Linter", "Texture_Mips_Fail", "Recommended: {0}"),
				(Texture->MipGenSettings == TMGS_NoMipmaps) ? FText::FromString(TEXT("Checked")) : FText::FromString(TEXT("UnChecked")));
			OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), RecommendedAction));
			return false;
		}
	}

	return true;
}
