// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Texture_NeverStream.h"

ULintRule_Texture_NeverStream::ULintRule_Texture_NeverStream(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DisableSteamInTheseGroups.Add(TextureGroup::TEXTUREGROUP_UI);
	DisableSteamInTheseGroups.Add(TextureGroup::TEXTUREGROUP_Pixels2D);
	DisableSteamInTheseGroups.Add(TextureGroup::TEXTUREGROUP_ColorLookupTable);
}

bool ULintRule_Texture_NeverStream::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	// If we aren't a texture, abort
	if (Cast<UTexture2D>(ObjectToLint) == nullptr)
	{
		// @TODO: Bubble up some sort of configuration error?
		return true;
	}

	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_Texture_NeverStream::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	const UTexture2D* Texture = CastChecked<UTexture2D>(ObjectToLint);
	bool bPreventNeverStream = DisableSteamInTheseGroups.Contains(Texture->LODGroup);
	if ((bPreventNeverStream ? 1 : 0) ^ (Texture->NeverStream ? 1 : 0))
	{
		FText RecommendedAction = FText::FormatOrdered(NSLOCTEXT("Linter", "Texture_NeverStream_Fail", "Recommended: {0}"),
			Texture->NeverStream ? FText::FromString(TEXT("UnChecked")) : FText::FromString(TEXT("Checked")));
		OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), RecommendedAction));
		return false;
	}

	return true;
}
