// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_SkelMesh_AnimUpdate.h"
#include "GameFramework/Actor.h"
#include "Engine/Blueprint.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SCS_Node.h"
#include "SuperUtilsBPLibrary.h"

ULintRule_SkelMesh_AnimUpdate::ULintRule_SkelMesh_AnimUpdate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULintRule_SkelMesh_AnimUpdate::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	// If we aren't a blueprint, abort
	if (Cast<UBlueprint>(ObjectToLint) == nullptr)
	{
		// @TODO: Bubble up some sort of configuration error?
		return true;
	}

	UBlueprint* Blueprint = CastChecked<UBlueprint>(ObjectToLint);
	UClass* GeneratedClass = Blueprint->GeneratedClass;
	if (!(GeneratedClass->IsChildOf<AActor>() || GeneratedClass->IsChildOf<USkeletalMeshComponent>()))
		return true;

	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_SkelMesh_AnimUpdate::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = false;
	UBlueprint* Blueprint = CastChecked<UBlueprint>(ObjectToLint);
	UClass* GeneratedClass = Blueprint->GeneratedClass;
	if (GeneratedClass->IsChildOf<USkeletalMeshComponent>())
	{
		if (!CheckAnimUpdate(Cast<USkeletalMeshComponent>(GeneratedClass->ClassDefaultObject)))
			bRuleViolated = true;
	}
	else
	{
		TArray<UActorComponent*> SMCs;
		USuperUtilsBPLibrary::GetComponentsInBlueprint(SMCs, USkeletalMeshComponent::StaticClass(), Blueprint);
		for (UActorComponent * ActorComponent : SMCs)
		{
			USkeletalMeshComponent* SMC = Cast<USkeletalMeshComponent>(ActorComponent);
			if (SMC)
			{
				if (!CheckAnimUpdate(Cast<USkeletalMeshComponent>(SMC)))
					bRuleViolated = true;
			}
		}
	}
	if (bRuleViolated)
	{
		FText RecommendedAction = NSLOCTEXT("Linter", "SkeletalMeshComponent AnimUpdate Failed", "Recommended: Do not use 'Always Tick Pose And Refresh Bones'");
		OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), RecommendedAction));
	}

	return !bRuleViolated;
}

bool ULintRule_SkelMesh_AnimUpdate::CheckAnimUpdate(USkeletalMeshComponent* SMC) const
{
	if (SMC && !SMC->IsEditorOnly()
		&& SMC->VisibilityBasedAnimTickOption == EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones)
		return false;
	return true;
}

