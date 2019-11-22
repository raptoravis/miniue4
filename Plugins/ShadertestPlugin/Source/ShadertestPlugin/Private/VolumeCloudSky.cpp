#include "VolumeCloudSky.h"

#include "Classes/Engine/TextureRenderTarget2D.h"
#include "Classes/Engine/World.h"
#include "Public/GlobalShader.h"
#include "Public/PipelineStateCache.h"
#include "Public/RHIStaticStates.h"
#include "Public/SceneUtils.h"
#include "Public/SceneInterface.h"
#include "Public/ShaderParameterUtils.h"
#include "Public/Logging/MessageLog.h"
#include "Public/Internationalization/Internationalization.h"
#include "Public/StaticBoundShaderState.h"

#include "CoreUObject.h"
#include "Engine.h"

#include "RHICommandList.h"
#include "UniformBuffer.h"

#define LOCTEXT_NAMESPACE "SDHVolumeCloudSky"

class FVolumeCSShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FVolumeCSShader, Global)

public:
	FVolumeCSShader()
	{
	}
	FVolumeCSShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) : FGlobalShader(Initializer)
	{
		// TODO Bind pramerter here
		CloudOutputSurface.Bind(Initializer.ParameterMap, TEXT("CloudOutputSurface"));
		TestStructureBufferSurface.Bind(Initializer.ParameterMap, TEXT("TestStructureBuffer"));
	}
	//----------------------------------------------------//
	static bool ShouldCache(EShaderPlatform PlateForm)
	{
		return IsFeatureLevelSupported(PlateForm, ERHIFeatureLevel::SM5);
	}
	//----------------------------------------------------//
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	//----------------------------------------------------//
	static void ModifyCompilationEnvironment(
		const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		// Define micro here
		// OutEnvironment.SetDefine(TEXT("TEST_MICRO"), 1);
	}
	//----------------------------------------------------//

	void SetSurface(
		FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef& OutputSurfaceUAV, FUnorderedAccessViewRHIRef& TestStructrueBuffUAV)
	{
		// set the UAV
		FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
		if (CloudOutputSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, CloudOutputSurface.GetBaseIndex(), OutputSurfaceUAV);
		if (TestStructureBufferSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, TestStructureBufferSurface.GetBaseIndex(), TestStructrueBuffUAV);
	}

	void UnBindBuffers(FRHICommandList& RHICmdList)
	{
		FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

		if (CloudOutputSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, CloudOutputSurface.GetBaseIndex(), FUnorderedAccessViewRHIRef());
		if (TestStructureBufferSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, TestStructureBufferSurface.GetBaseIndex(), FUnorderedAccessViewRHIRef());
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		// Serrilize something here
		Ar << CloudOutputSurface << TestStructureBufferSurface;
		return bShaderHasOutdatedParameters;
	}

private:
	FShaderResourceParameter CloudOutputSurface;
	FShaderResourceParameter TestStructureBufferSurface;
};

IMPLEMENT_SHADER_TYPE(, FVolumeCSShader, TEXT("/Plugin/ShadertestPlugin/Private/VolumeCloudSky.usf"), TEXT("MainCS"), SF_Compute)

void AVolumetricClouds::RenderCloud_RenderThread(
	FRHICommandListImmediate& RHICmdList, ERHIFeatureLevel::Type FeatureLevel, FRHITexture* RenderTarget, int32 SizeX, int32 SizeY)
{
	check(IsInRenderingThread());
	check(RenderTarget != nullptr);

	TShaderMapRef<FVolumeCSShader> VolumetricCloudComputeShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(VolumetricCloudComputeShader->GetComputeShader());

	// Init
	if (RenderCloud_RenderThread_Init == false || Texture.IsValid() == false || TextureUAV.IsValid() == false)
	{
		FRHIResourceCreateInfo CreateInfo;
		Texture = RHICreateTexture2D(SizeX, SizeY, PF_FloatRGBA, 1, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
		TextureUAV = RHICreateUnorderedAccessView(Texture);

		//*******************************************************//
		TestStruct TestElement;
		TestElement.TestPosition = FVector(1.0f, 1.0f, 1.0f);
		TResourceArray<TestStruct> bufferData;
		bufferData.Reset();
		bufferData.Add(TestElement);
		bufferData.SetAllowCPUAccess(true);

		FRHIResourceCreateInfo TestcreateInfo;
		TestcreateInfo.ResourceArray = &bufferData;

		TestStructureBuff = RHICreateStructuredBuffer(
			sizeof(TestStruct), sizeof(TestStruct) * 1, BUF_UnorderedAccess | BUF_ShaderResource, TestcreateInfo);
		TestStructureBuffUAV = RHICreateUnorderedAccessView(TestStructureBuff, true, false);

		//*******************************************************//

		RenderCloud_RenderThread_Init = true;
	}

	VolumetricCloudComputeShader->SetSurface(RHICmdList, TextureUAV, TestStructureBuffUAV);
	DispatchComputeShader(RHICmdList, *VolumetricCloudComputeShader, SizeX / 32, SizeY / 32, 1);
	VolumetricCloudComputeShader->UnBindBuffers(RHICmdList);

	FRHICopyTextureInfo copyInfo;
	copyInfo.Size.X = SizeX;
	copyInfo.Size.Y = SizeY;

	RHICmdList.CopyTexture(Texture, RenderTarget, copyInfo);

	TArray<FVector> Data;
	Data.Reset();
	FVector TestEle(1.0f, 1.0f, 1.0f);
	Data.Add(TestEle);

	FVector* srcptr =
		(FVector*) RHILockStructuredBuffer(TestStructureBuff.GetReference(), 0, sizeof(FVector), EResourceLockMode::RLM_ReadOnly);
	FMemory::Memcpy(Data.GetData(), srcptr, sizeof(FVector));
	RHIUnlockStructuredBuffer(TestStructureBuff.GetReference());
}

void AVolumetricClouds::RenderCloud(UTextureRenderTarget2D* RenderTarget)
{
	UWorld* world = GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = world->Scene->GetFeatureLevel();
	checkf(FeatureLevel == ERHIFeatureLevel::SM5, TEXT("Only surpport SM5"));

	FTextureReferenceRHIRef RenderTargetTextureRHI = RenderTarget->TextureReference.TextureReferenceRHI;
	checkf(RenderTargetTextureRHI != nullptr, TEXT("Can't get render target %d texture"));

	FRHITexture* RenderTargetTextureRef = RenderTargetTextureRHI->GetTextureReference()->GetReferencedTexture();
	int32 SizeX = RenderTarget->SizeX;
	int32 SizeY = RenderTarget->SizeY;

	ENQUEUE_RENDER_COMMAND(VolumeCloudsRenderCommand)
	([FeatureLevel, RenderTargetTextureRef, SizeX, SizeY, this](FRHICommandListImmediate& RHICmdList) {
		RenderCloud_RenderThread(RHICmdList, FeatureLevel, RenderTargetTextureRef, SizeX, SizeY);
	});
}

#undef LOCTEXT_NAMESPACE