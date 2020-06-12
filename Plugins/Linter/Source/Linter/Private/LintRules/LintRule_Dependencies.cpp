// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Dependencies.h"
#include "AssetRegistryModule.h"

ULintRule_Dependencies::ULintRule_Dependencies(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//bRequiresGameThread = true;
}

bool ULintRule_Dependencies::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_Dependencies::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;
	FString PackageName = ObjectToLint->GetOutermost()->GetName();
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FName> UnfilteredDependencies;
	AssetRegistry.GetDependencies(*PackageName, UnfilteredDependencies);
	if (UnfilteredDependencies.Num())
	{
		FText RecommendedAction = NSLOCTEXT("Linter", "Dependencies", "Lost: ");
		TArray<FText> Messages;
		Messages.Add(RecommendedAction);

		for (auto NameDependence : UnfilteredDependencies)
		{
			FString StrDependence(NameDependence.ToString());
			if (FPackageName::IsScriptPackage(StrDependence))
				continue;

			if (!FPackageName::DoesPackageExist(NameDependence.ToString(), nullptr))
			{
				Messages.Add(FText::FromString(StrDependence));
				bRuleViolated = false;
			}
		}

		if (!bRuleViolated)
		{
			FText ResultMessage = FText::Join(FText::FromString(TEXT(", ")), Messages);
			OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), ResultMessage));
		}
	}

	return bRuleViolated;
}
