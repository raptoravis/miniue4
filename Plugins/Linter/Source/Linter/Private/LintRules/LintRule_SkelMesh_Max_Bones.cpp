// Fill out your copyright notice in the Description page of Project Settings.


#include "LintRules/LintRule_SkelMesh_Max_Bones.h"
#include "Engine/SkeletalMesh.h"
#include "Rendering/SkeletalMeshRenderData.h"

ULintRule_SkelMesh_Max_Bones::ULintRule_SkelMesh_Max_Bones(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

bool ULintRule_SkelMesh_Max_Bones::PassesRule(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	USkeletalMesh* pSkeletalMesh = Cast<USkeletalMesh>(ObjectToLint);
	if (!pSkeletalMesh)
		return true;
	return Super::PassesRule(ObjectToLint, ParentRuleSet, OutRuleViolations);
}

bool ULintRule_SkelMesh_Max_Bones::PassesRule_Internal_Implementation(UObject* ObjectToLint, const ULintRuleSet* ParentRuleSet, TArray<FLintRuleViolation>& OutRuleViolations) const
{
	bool bRuleViolated = true;
	USkeletalMesh* pSkeletalMesh = CastChecked<USkeletalMesh>(ObjectToLint);
	FSkeletalMeshRenderData* SkelMeshRenderDate = pSkeletalMesh->GetResourceForRendering();
	if (SkelMeshRenderDate)
	{
		TArray<FText> Messages;
		FText RecommendedAction = FText::FormatOrdered(NSLOCTEXT("Linter", "SkeletalMesh Max Bones", "Max allowed Bones: {0}."), FText::AsNumber(MaxBones));
		Messages.Add(RecommendedAction);

		for (int32 iLODRenderData = 0; iLODRenderData < SkelMeshRenderDate->LODRenderData.Num(); ++iLODRenderData)
		{
			FSkeletalMeshLODRenderData& LODData = SkelMeshRenderDate->LODRenderData[iLODRenderData];
			if (LODData.RequiredBones.Num() > MaxBones)
			{
				Messages.Add(FText::FormatOrdered(NSLOCTEXT("Linter", "SkeletalMesh Bones", "Bones: LOD{0}, Count {1}."),
					FText::AsNumber(iLODRenderData), 
					FText::AsNumber(LODData.RequiredBones.Num()))
				);
				bRuleViolated = false;
			}
		}

		if (!bRuleViolated)
		{
			FText ResultMessage = FText::Join(FText::FromString(TEXT("\n")), Messages);
			OutRuleViolations.Push(FLintRuleViolation(ObjectToLint, GetClass(), ResultMessage));
		}
	}

	return bRuleViolated;
}
