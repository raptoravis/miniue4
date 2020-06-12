// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SuperUtilsBPLibrary.h"
#include "SuperUtils.h"
#include "Engine/Blueprint.h"
#include "Components/ActorComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Materials/Material.h"
#include "CoreGlobals.h"

USuperUtilsBPLibrary::USuperUtilsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USuperUtilsBPLibrary::SuperUtilsSampleFunction(float Param)
{
	return -1;
}

void USuperUtilsBPLibrary::GetComponentsInBlueprint(TArray<UActorComponent*>& OutComponents, UClass* ActorComponentClass, UBlueprint* Blueprint)
{
	if (!Cast<UBlueprint>(Blueprint))
	{
		UE_LOG(LogSuperUtils, Error, TEXT("Param can not cast to class'UBlueprint'"));
		return;
	}

	UClass* GeneratedClass = Blueprint->GeneratedClass;
	UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(GeneratedClass);
	if (BPGC && BPGC->SimpleConstructionScript)
	{
		const TArray<USCS_Node*>& AllSCSNodes = BPGC->SimpleConstructionScript->GetAllNodes();
		for (USCS_Node* SCSNode : AllSCSNodes)
		{
			UActorComponent* ActorComponent = SCSNode->GetActualComponentTemplate(BPGC);
			if (ActorComponent->IsA(ActorComponentClass))
				OutComponents.Add(ActorComponent);
		}
	}
}

void USuperUtilsBPLibrary::GetMaterialInstructionCounts(TSet<ERHIFeatureLevel::Type> FeatureLevels, UMaterial* Material, 
	TMap<ERHIFeatureLevel::Type, TArray<FMaterialStatsUtils::FShaderInstructionsInfo>>& Results)
{
	if (!IsRunningCommandlet())
		return;

	UMaterialInterface::SetGlobalRequiredFeatureLevel(ERHIFeatureLevel::ES2, FeatureLevels.Contains(ERHIFeatureLevel::ES2));
	UMaterialInterface::SetGlobalRequiredFeatureLevel(ERHIFeatureLevel::SM5, FeatureLevels.Contains(ERHIFeatureLevel::SM5));
	UMaterialInterface::SetGlobalRequiredFeatureLevel(ERHIFeatureLevel::ES3_1, FeatureLevels.Contains(ERHIFeatureLevel::ES3_1));

	for (ERHIFeatureLevel::Type FeatureLevel : FeatureLevels)
	{
		TArray<FMaterialStatsUtils::FShaderInstructionsInfo>& Result = Results.FindOrAdd(FeatureLevel);
		FMaterialResource* MaterialResource = Material->GetMaterialResource(FeatureLevel);
		if (MaterialResource)
		{
			MaterialResource->FinishCompilation();
			FMaterialStatsUtils::GetRepresentativeInstructionCounts(Result, MaterialResource);
		}
	}
}

