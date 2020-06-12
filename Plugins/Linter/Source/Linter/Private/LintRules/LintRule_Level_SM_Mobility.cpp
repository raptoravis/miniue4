// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_Level_SM_Mobility.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"

ULintRule_Level_SM_Mobility::ULintRule_Level_SM_Mobility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULintRule_Level_SM_Mobility::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;
	UWorld* World = Cast<UWorld>(ObjectToLint);
	if (ULevel* Level = World->PersistentLevel)
	{
		FText RecommendedAction = NSLOCTEXT("Linter", "StaticMesh Mobility", "Actors");
		TArray<FText> Messages;
		Messages.Add(RecommendedAction);

		for (AActor* Actor : Level->Actors)
		{
			if (Actor && Actor->IsA<AStaticMeshActor>() && Actor->IsRootComponentMovable())
			{
				Messages.Add(FText::FromString(Actor->GetFName().ToString()));
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
