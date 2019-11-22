#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "VolumeCloudSky.generated.h"

struct TestStruct
{
	FVector TestPosition;
};

UCLASS()
class AVolumetricClouds : public AActor
{
	GENERATED_BODY()

public:
	AVolumetricClouds()
	{
	}

	UFUNCTION(BlueprintCallable, Category = "VolumeCloud")
	void RenderCloud(UTextureRenderTarget2D* RenderTarget);

private:
	void RenderCloud_RenderThread(FRHICommandListImmediate& RHICmdList, ERHIFeatureLevel::Type FeatureLevel,
		FRHITexture* RenderTarget, int32 SizeX, int32 SizeY);

	FTexture2DRHIRef Texture;
	FUnorderedAccessViewRHIRef TextureUAV;

	FStructuredBufferRHIRef TestStructureBuff;
	FUnorderedAccessViewRHIRef TestStructureBuffUAV;

	bool RenderCloud_RenderThread_Init;
};