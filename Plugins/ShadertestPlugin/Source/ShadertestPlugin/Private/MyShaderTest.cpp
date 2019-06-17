// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.  

#include "MyShaderTest.h"  

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

#include "RHICommandList.h"  
#include "UniformBuffer.h"  


#define LOCTEXT_NAMESPACE "TestShader"  


BEGIN_GLOBAL_SHADER_PARAMETER_STRUCT(FMyUniformStructData, )
	SHADER_PARAMETER(FVector4, ColorOne)
	SHADER_PARAMETER(FVector4, ColorTwo)
	SHADER_PARAMETER(FVector4, ColorThree)
	SHADER_PARAMETER(FVector4, ColorFour)
	SHADER_PARAMETER(uint32, ColorIndex)
END_GLOBAL_SHADER_PARAMETER_STRUCT()

IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FMyUniformStructData, "FMyUniform");


UTestShaderBlueprintLibrary::UTestShaderBlueprintLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


class FMyShaderTest : public FGlobalShader
{
public:

	FMyShaderTest() {}

	FMyShaderTest(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		SimpleColorVal.Bind(Initializer.ParameterMap, TEXT("SimpleColor"));
		TestTextureVal.Bind(Initializer.ParameterMap, TEXT("MyTexture"));
		TestTextureSampler.Bind(Initializer.ParameterMap, TEXT("MyTextureSampler"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		//return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM4);  
		return true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("TEST_MICRO"), 1);
	}

	void SetParameters(
		FRHICommandListImmediate& RHICmdList,
		const FLinearColor& MyColor,
		FTextureRHIParamRef& MyTexture, 
		FMyShaderStructData& ShaderStructData
	)
	{
		SetShaderValue(RHICmdList, GetPixelShader(), SimpleColorVal, MyColor);
		SetTextureParameter(
			RHICmdList,
			GetPixelShader(),
			TestTextureVal,
			TestTextureSampler,
			TStaticSamplerState<SF_Trilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI(),
			MyTexture);

		FMyUniformStructData UniformData;
		UniformData.ColorOne = ShaderStructData.ColorOne;
		UniformData.ColorTwo = ShaderStructData.ColorTwo;
		UniformData.ColorThree = ShaderStructData.Colorthree;
		UniformData.ColorFour = ShaderStructData.ColorFour;
		UniformData.ColorIndex = ShaderStructData.ColorIndex;
		UniformData.ColorIndex = ShaderStructData.ColorIndex;

		SetUniformBufferParameterImmediate(RHICmdList, GetPixelShader(), GetUniformBufferParameter<FMyUniformStructData>(), UniformData);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << SimpleColorVal << TestTextureVal << TestTextureSampler;
		return bShaderHasOutdatedParameters;
	}

private:

	FShaderParameter SimpleColorVal;

	FShaderResourceParameter TestTextureVal;
	FShaderResourceParameter TestTextureSampler;

};

class FShaderTestVS : public FMyShaderTest
{
	DECLARE_SHADER_TYPE(FShaderTestVS, Global);

public:
	FShaderTestVS() {}

	FShaderTestVS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMyShaderTest(Initializer)
	{

	}
};


class FShaderTestPS : public FMyShaderTest
{
	DECLARE_SHADER_TYPE(FShaderTestPS, Global);

public:
	FShaderTestPS() {}

	FShaderTestPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FMyShaderTest(Initializer)
	{

	}
};

//***************************************************************************************//
class FMyComputeShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FMyComputeShader, Global);

public:

	FMyComputeShader() {
	}

	FMyComputeShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		:FGlobalShader(Initializer)
	{
		OutputSurface.Bind(Initializer.ParameterMap, TEXT("OutputSurface"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
		//return true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Platform, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);

		Ar << OutputSurface;

		return bShaderHasOutdatedParams;
	}

	void SetSurfaces(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutputSurfaceUAV, FMyShaderStructData& ShaderStructData)
	{
		FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
		if (OutputSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, OutputSurface.GetBaseIndex(), OutputSurfaceUAV);

		FMyUniformStructData UniformData;
		UniformData.ColorOne = ShaderStructData.ColorOne;
		UniformData.ColorTwo = ShaderStructData.ColorTwo;
		UniformData.ColorThree = ShaderStructData.Colorthree;
		UniformData.ColorFour = ShaderStructData.ColorFour;
		UniformData.ColorIndex = ShaderStructData.ColorIndex;

		SetUniformBufferParameterImmediate(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FMyUniformStructData>(), UniformData);

	}

	void UnbindBuffers(FRHICommandList& RHICmdList)
	{
		FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

		if (OutputSurface.IsBound())
			RHICmdList.SetUAVParameter(ComputeShaderRHI, OutputSurface.GetBaseIndex(), FUnorderedAccessViewRHIRef());
	}

private:
	FShaderResourceParameter OutputSurface;
};

IMPLEMENT_SHADER_TYPE(, FShaderTestVS, TEXT("/Plugin/ShadertestPlugin/Private/MyShader.usf"), TEXT("MainVS"), SF_Vertex)
IMPLEMENT_SHADER_TYPE(, FShaderTestPS, TEXT("/Plugin/ShadertestPlugin/Private/MyShader.usf"), TEXT("MainPS"), SF_Pixel)
IMPLEMENT_SHADER_TYPE(, FMyComputeShader, TEXT("/Plugin/ShadertestPlugin/Private/MyShader.usf"), TEXT("MainCS"), SF_Compute)

struct FMyTextureVertex
{
	FVector4    Position;
	FVector2D   UV;
};

class FMyTextureVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FMyTextureVertex);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyTextureVertex, Position), VET_Float4, 0, Stride));
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FMyTextureVertex, UV), VET_Float2, 1, Stride));
		VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
	}

	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI->Release();
	}
};


static void DrawTestShaderRenderTarget_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	ERHIFeatureLevel::Type FeatureLevel,
	FName TextureRenderTargetName,
	FLinearColor MyColor, 
	FTextureRHIParamRef MyTexture,
	FMyShaderStructData ShaderStructData
)
{
	check(IsInRenderingThread());

#if WANTS_DRAW_MESH_EVENTS  
	FString EventName;
	TextureRenderTargetName.ToString(EventName);
	SCOPED_DRAW_EVENTF(RHICmdList, SceneCapture, TEXT("ShaderTest %s"), *EventName);
#else  
	SCOPED_DRAW_EVENT(RHICmdList, DrawUVDisplacementToRenderTarget_RenderThread);
#endif  

	//设置渲染目标  
	//SetRenderTarget(
	//	RHICmdList,
	//	OutputRenderTargetResource->GetRenderTargetTexture(),
	//	FTextureRHIRef(),
	//	ESimpleRenderTargetMode::EUninitializedColorAndDepth,
	//	FExclusiveDepthStencil::DepthNop_StencilNop
	//);

	FRHIRenderPassInfo RPInfo(OutputRenderTargetResource->GetRenderTargetTexture(), ERenderTargetActions::DontLoad_Store, OutputRenderTargetResource->TextureRHI);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawTestShader"));

	//设置视口  
	//FIntPoint DrawTargetResolution(OutputRenderTargetResource->GetSizeX(), OutputRenderTargetResource->GetSizeY());  
	//RHICmdList.SetViewport(0, 0, 0.0f, DrawTargetResolution.X, DrawTargetResolution.Y, 1.0f);  

	TShaderMap<FGlobalShaderType>* GlobalShaderMap = GetGlobalShaderMap(FeatureLevel);
	TShaderMapRef<FShaderTestVS> VertexShader(GlobalShaderMap);
	TShaderMapRef<FShaderTestPS> PixelShader(GlobalShaderMap);

	FMyTextureVertexDeclaration VertexDec;
	VertexDec.InitRHI();


	// Set the graphic pipeline state.  
	FGraphicsPipelineStateInitializer GraphicsPSOInit;
	RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
	GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
	GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
	GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
	GraphicsPSOInit.PrimitiveType = PT_TriangleList;
	//GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDec.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexShaderRHI = GETSAFERHISHADER_VERTEX(*VertexShader);
	GraphicsPSOInit.BoundShaderState.PixelShaderRHI = GETSAFERHISHADER_PIXEL(*PixelShader);
	SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	//RHICmdList.SetViewport(0, 0, 0.0f, DrawTargetResolution.X, DrawTargetResolution.Y, 1.0f);  
	PixelShader->SetParameters(RHICmdList, MyColor, MyTexture, ShaderStructData);

	// Draw grid.  
	//uint32 PrimitiveCount = 2;  
	//RHICmdList.DrawPrimitive(PT_TriangleList, 0, PrimitiveCount, 1);  
	FMyTextureVertex Vertices[4];
	Vertices[0].Position.Set(-1.0f, 1.0f, 0, 1.0f);
	Vertices[1].Position.Set(1.0f, 1.0f, 0, 1.0f);
	Vertices[2].Position.Set(-1.0f, -1.0f, 0, 1.0f);
	Vertices[3].Position.Set(1.0f, -1.0f, 0, 1.0f);
	Vertices[0].UV = FVector2D(0.0f, 1.0f);
	Vertices[1].UV = FVector2D(1.0f, 1.0f);
	Vertices[2].UV = FVector2D(0.0f, 0.0f);
	Vertices[3].UV = FVector2D(1.0f, 0.0f);

	static const uint16 Indices[6] =
	{
		0, 1, 2,
		2, 1, 3
	};
	//DrawPrimitiveUP(RHICmdList, PT_TriangleStrip, 2, Vertices, sizeof(Vertices[0]));  
	//DrawIndexedPrimitiveUP(
	//	RHICmdList,
	//	PT_TriangleList,
	//	0,
	//	ARRAY_COUNT(Vertices),
	//	2,
	//	Indices,
	//	sizeof(Indices[0]),
	//	Vertices,
	//	sizeof(Vertices[0])
	//);
	{
		uint32 PrimitiveType = PT_TriangleList;
		uint32 MinVertexIndex = 0;
		uint32 NumVertices = ARRAY_COUNT(Vertices);
		uint32 NumPrimitives = 2;
		const void* IndexData = Indices;
		uint32 IndexDataStride = sizeof(Indices[0]);
		const void* VertexData = Vertices;
		uint32 VertexDataStride = sizeof(Vertices[0]);

		const uint32 NumIndices = GetVertexCountForPrimitiveCount(NumPrimitives, PrimitiveType);

		FRHIResourceCreateInfo CreateInfo;
		FVertexBufferRHIRef VertexBufferRHI = RHICreateVertexBuffer(VertexDataStride * NumVertices, BUF_Volatile, CreateInfo);
		void* VoidPtr = RHILockVertexBuffer(VertexBufferRHI, 0, VertexDataStride * NumVertices, RLM_WriteOnly);
		FPlatformMemory::Memcpy(VoidPtr, VertexData, VertexDataStride * NumVertices);
		RHIUnlockVertexBuffer(VertexBufferRHI);

		FIndexBufferRHIRef IndexBufferRHI = RHICreateIndexBuffer(IndexDataStride, IndexDataStride * NumIndices, BUF_Volatile, CreateInfo);
		void* VoidPtr2 = RHILockIndexBuffer(IndexBufferRHI, 0, IndexDataStride * NumIndices, RLM_WriteOnly);
		FPlatformMemory::Memcpy(VoidPtr2, IndexData, IndexDataStride * NumIndices);
		RHIUnlockIndexBuffer(IndexBufferRHI);

		RHICmdList.SetStreamSource(0, VertexBufferRHI, 0);
		RHICmdList.DrawIndexedPrimitive(IndexBufferRHI, MinVertexIndex, 0, NumVertices, 0, NumPrimitives, 1);

		IndexBufferRHI.SafeRelease();
		VertexBufferRHI.SafeRelease();
	}

	//// Resolve render target.  
	//RHICmdList.CopyToResolveTarget(
	//	OutputRenderTargetResource->GetRenderTargetTexture(),
	//	OutputRenderTargetResource->TextureRHI,
	//	FResolveParams());
	RHICmdList.EndRenderPass();
}

void UTestShaderBlueprintLibrary::DrawTestShaderRenderTarget(
	UTextureRenderTarget2D* OutputRenderTarget,
	AActor* Ac,
	FLinearColor MyColor, 
	UTexture* MyTexture,
	FMyShaderStructData ShaderStructData
)
{
	check(IsInGameThread());

	if (!OutputRenderTarget)
	{
		return;
	}

	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
	FTextureRHIParamRef MyTextureRHI = MyTexture->TextureReference.TextureReferenceRHI;
	UWorld* World = Ac->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FName TextureRenderTargetName = OutputRenderTarget->GetFName();
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[TextureRenderTargetResource, FeatureLevel, MyColor, MyTextureRHI, ShaderStructData, TextureRenderTargetName](FRHICommandListImmediate& RHICmdList)
		{
			DrawTestShaderRenderTarget_RenderThread(RHICmdList, TextureRenderTargetResource, FeatureLevel, 
				TextureRenderTargetName, MyColor, MyTextureRHI, ShaderStructData);
		}
	);

}

static void TextureWriting_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	ERHIFeatureLevel::Type FeatureLevel,
	UTexture2D* Texture
)
{
	check(IsInRenderingThread());
	if (Texture == nullptr)
	{
		return;
	}

	FTextureReferenceRHIRef MyTextureRHI = Texture->TextureReference.TextureReferenceRHI;
	FRHITexture* TexRef = MyTextureRHI->GetTextureReference()->GetReferencedTexture();
	FRHITexture2D* TexRef2D = (FRHITexture2D*)TexRef;

	TArray<FColor> Bitmap;
	//TArray<uint32> sourcedata;
	//-----------------------------------
	uint32 LolStride = 0;
	char* TextureDataPtr = (char*)RHICmdList.LockTexture2D(TexRef2D, 0, EResourceLockMode::RLM_WriteOnly, LolStride, false);

	for (uint32 Row = 0; Row < TexRef2D->GetSizeY(); ++Row)
	{
		uint32* PixelPtr = (uint32*)TextureDataPtr;
		for (uint32 Col = 0; Col < TexRef2D->GetSizeX(); ++Col)
		{
			uint32 EncodedPixel = *PixelPtr;
			uint8 r = 255;
			uint8 g = 0;
			uint8 b = 0;
			uint8 a = 255;
			*PixelPtr = r | (g << 8) | (b << 16) | (a << 24);
			//sourcedata.Add(*PixelPtr);
			PixelPtr++;
		}
		// move to next row:
		TextureDataPtr += LolStride;
	}
	RHICmdList.UnlockTexture2D(TexRef2D, 0, false);

	//FUpdateTextureRegion2D region = FUpdateTextureRegion2D(0, 0, 0, 0, TexRef2D->GetSizeX(), TexRef2D->GetSizeY());
	//RHIUpdateTexture2D(TexRef2D, 0, region, sizeof(uint32) * TexRef2D->GetSizeX() * TexRef2D->GetSizeY(), (uint8*)sourcedata.GetData());

	//-----------------------------------
	//Bitmap.Reset();
	TextureDataPtr = (char*)RHICmdList.LockTexture2D(TexRef2D, 0, EResourceLockMode::RLM_ReadOnly, LolStride, false);

	for (uint32 Row = 0; Row < TexRef2D->GetSizeY(); ++Row)
	{
		uint32* PixelPtr = (uint32*)TextureDataPtr;
		for (uint32 Col = 0; Col < TexRef2D->GetSizeX(); ++Col)
		{
			uint32 EncodedPixel = *PixelPtr;
			uint8 r = (EncodedPixel & 0x000000FF);
			uint8 g = (EncodedPixel & 0x0000FF00) >> 8;
			uint8 b = (EncodedPixel & 0x00FF0000) >> 16;
			uint8 a = (EncodedPixel & 0xFF000000) >> 24;
			FColor col = FColor(r, g, b, a);
			Bitmap.Add(FColor(b, g, r, a));
			PixelPtr++;
		}
		// move to next row:
		TextureDataPtr += LolStride;
	}
	RHICmdList.UnlockTexture2D(TexRef2D, 0, false);

	if (Bitmap.Num())
	{
		IFileManager::Get().MakeDirectory(*FPaths::ScreenShotDir(), true);
		const FString ScreenFileName(FPaths::ScreenShotDir() / TEXT("VisualizeTexture"));
		uint32 ExtendXWithMSAA = Bitmap.Num() / Texture->GetSizeY();
		// Save the contents of the array to a bitmap file. (24bit only so alpha channel is dropped)
		FFileHelper::CreateBitmap(*ScreenFileName, ExtendXWithMSAA, Texture->GetSizeY(), Bitmap.GetData());
		UE_LOG(LogConsoleResponse, Display, TEXT("Content was saved to \"%s\""), *FPaths::ScreenShotDir());
	}
	else
	{
		UE_LOG(LogConsoleResponse, Error, TEXT("Failed to save BMP, format or texture type is not supported"));
	}
}

void UTestShaderBlueprintLibrary::TextureWriting(UTexture2D* TextureToBeWrite, AActor* selfref)
{
	check(IsInGameThread());

	if (selfref == nullptr && TextureToBeWrite == nullptr)return;

	//TextureToBeWrite->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	//TextureToBeWrite->SRGB = 0;

	//FTexture2DMipMap& mipmap = TextureToBeWrite->PlatformData->Mips[0];
	//void* Data = mipmap.BulkData.Lock(LOCK_READ_WRITE);

	//int32 texturex = TextureToBeWrite->PlatformData->SizeX;
	//int32 texturey = TextureToBeWrite->PlatformData->SizeY;
	//TArray<FColor>colors;
	//for (int32 x = 0; x < texturex * texturey; x++)
	//{
	//	colors.Add(FColor::Blue);
	//}
	//int32 stride = (int32)(sizeof(uint8) * 4);
	//FMemory::Memcpy(Data, colors.GetData(), texturex * texturey * stride);
	//mipmap.BulkData.Unlock();
	//TextureToBeWrite->UpdateResource();

	/*
		struct FUpdateTextureContext
		{
			uint8* SourceBuffer;
			uint32 BufferPitch;
			FTexture2DResource* DestTextureResource;
		};

		FUpdateTextureContext UpdateTextureContext = {
			uint8*()
		}

		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
			UpdateDynamicTexture,
			FUpdateTexture
		);
	*/
	UWorld* World = selfref->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[FeatureLevel, TextureToBeWrite](FRHICommandListImmediate& RHICmdList)
		{
			TextureWriting_RenderThread
			(
				RHICmdList,
				FeatureLevel,
				TextureToBeWrite
			);
		}
	);

}

class FMyComputeShaderTest : public FGlobalShader
{
public:

	FMyComputeShaderTest() {}

	FMyComputeShaderTest(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
		: FGlobalShader(Initializer)
	{
		OutputSurface.Bind(Initializer.ParameterMap, TEXT("OutputSurface"));
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		//return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM4);  
		return true;
	}

	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("TEST_MICRO"), 1);
	}

	void SetSurfaces(
		FRHICommandListImmediate& RHICmdList,
		FUnorderedAccessViewRHIParamRef& OutputSurfaceUAV)
	{
		if (OutputSurface.IsBound())
		{
			FComputeShaderRHIParamRef ref = (FRHIComputeShader*)this;
			RHICmdList.SetUAVParameter(ref, OutputSurface.GetBaseIndex(), OutputSurfaceUAV);
		}
	}

	void UnbindBuffers(
		FRHICommandListImmediate& RHICmdList)
	{
		if (OutputSurface.IsBound())
		{
			FComputeShaderRHIParamRef ref = (FRHIComputeShader*)this;
			RHICmdList.SetUAVParameter(ref, OutputSurface.GetBaseIndex(), FUnorderedAccessViewRHIParamRef());
		}
	}

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << OutputSurface;
		return bShaderHasOutdatedParameters;
	}

private:
	FShaderResourceParameter OutputSurface;

};

void UTestShaderBlueprintLibrary::CopySurfaceToTexture(UTextureRenderTarget2D* OutRenderTarget)
{
	FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();
	int32 SizeX = OutRenderTarget->SizeX;
	int32 SizeY = OutRenderTarget->SizeY;

	FRHIResourceCreateInfo CreateInfo;
	FTexture2DRHIRef Texture = RHICreateTexture2D(SizeX, SizeY, PF_A32B32G32R32F, 1, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	FUnorderedAccessViewRHIParamRef TextureUAV = RHICreateUnorderedAccessView(Texture);

	FMyComputeShaderTest* ComputeShader = (FMyComputeShaderTest*)0;
	ComputeShader->SetSurfaces(RHICmdList, TextureUAV);
	DispatchComputeShader(RHICmdList, ComputeShader, SizeX / 32, SizeY / 32, 1);
	ComputeShader->UnbindBuffers(RHICmdList);

	FTextureReferenceRHIRef TestRTTextureRef = OutRenderTarget->TextureReference.TextureReferenceRHI;
	FRHITexture* RTTexture = TestRTTextureRef->GetTextureReference()->GetReferencedTexture();
	if (!RTTexture) {
		return;
	}

	FRHICopyTextureInfo copyInfo;
	copyInfo.Size.X = SizeX;
	copyInfo.Size.Y = SizeY;

	RHICmdList.CopyTexture(Texture, RTTexture, copyInfo);

}


/*****************************************************/

static void UseComputeShader_RenderThread(
	FRHICommandListImmediate& RHICmdList,
	FTextureRenderTargetResource* OutputRenderTargetResource,
	FName TextureRenderTargetName, 
	FMyShaderStructData ShaderStructData,
	ERHIFeatureLevel::Type FeatureLevel
)
{
	check(IsInRenderingThread());

	TShaderMapRef<FMyComputeShader> ComputeShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(ComputeShader->GetComputeShader());

	//ComputeShader->SetSurfaces(RHICmdList,)
	int32 SizeX = OutputRenderTargetResource->GetSizeX();
	int32 SizeY = OutputRenderTargetResource->GetSizeY();
	FRHIResourceCreateInfo CreateInfo;

	FTexture2DRHIRef Texture = RHICreateTexture2D(SizeX, SizeY, PF_A32B32G32R32F, 1, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	FUnorderedAccessViewRHIRef TextureUAV = RHICreateUnorderedAccessView(Texture);
	ComputeShader->SetSurfaces(RHICmdList, TextureUAV, ShaderStructData);
	DispatchComputeShader(RHICmdList, *ComputeShader, SizeX / 32, SizeY / 32, 1);
	ComputeShader->UnbindBuffers(RHICmdList);

	DrawTestShaderRenderTarget_RenderThread(RHICmdList, OutputRenderTargetResource, FeatureLevel, 
		TextureRenderTargetName, FLinearColor(), Texture, ShaderStructData);

	/*
	//create a bitmap
	TArray<FColor> Bitmap;

	//To access our resource we do a custom read using lockrect
	uint32 LolStride = 0;
	char* TextureDataPtr = (char*)RHICmdList.LockTexture2D(Texture, 0, EResourceLockMode::RLM_ReadOnly, LolStride, false);

	for (uint32 Row = 0; Row < Texture->GetSizeY(); ++Row)
	{
		uint32* PixelPtr = (uint32*)TextureDataPtr;

		//Since we are using our custom UINT format, we need to unpack it here to access the actual colors
		for (uint32 Col = 0; Col < Texture->GetSizeX(); ++Col)
		{
			uint32 EncodedPixel = *PixelPtr;
			uint8 r = (EncodedPixel & 0x000000FF);
			uint8 g = (EncodedPixel & 0x0000FF00) >> 8;
			uint8 b = (EncodedPixel & 0x00FF0000) >> 16;
			uint8 a = (EncodedPixel & 0xFF000000) >> 24;
			Bitmap.Add(FColor(r, g, b, a));

			PixelPtr++;
		}

		// move to next row:
		TextureDataPtr += LolStride;
	}

	RHICmdList.UnlockTexture2D(Texture, 0, false);

	// if the format and texture type is supported
	if (Bitmap.Num())
	{
		// Create screenshot folder if not already present.
		IFileManager::Get().MakeDirectory(*FPaths::ScreenShotDir(), true);

		const FString ScreenFileName(FPaths::ScreenShotDir() / TEXT("VisualizeTexture"));

		uint32 ExtendXWithMSAA = Bitmap.Num() / Texture->GetSizeY();

		// Save the contents of the array to a bitmap file. (24bit only so alpha channel is dropped)
		FFileHelper::CreateBitmap(*ScreenFileName, ExtendXWithMSAA, Texture->GetSizeY(), Bitmap.GetData());

		UE_LOG(LogConsoleResponse, Display, TEXT("Content was saved to \"%s\""), *FPaths::ScreenShotDir());
	}
	else
	{
		UE_LOG(LogConsoleResponse, Error, TEXT("Failed to save BMP, format or texture type is not supported"));
	}

	*/
}


void UTestShaderBlueprintLibrary::UseMyComputeShader(
	class UTextureRenderTarget2D* OutputRenderTarget,
	AActor* Ac,
	FMyShaderStructData ShaderStructData
)
{
	check(IsInGameThread());

	if (Ac == nullptr && OutputRenderTarget == nullptr)
	{
		return;
	}

	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->GameThread_GetRenderTargetResource();
	UWorld* World = Ac->GetWorld();
	ERHIFeatureLevel::Type FeatureLevel = World->Scene->GetFeatureLevel();
	FName TextureRenderTargetName = OutputRenderTarget->GetFName();

	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[TextureRenderTargetResource, TextureRenderTargetName, FeatureLevel, ShaderStructData](FRHICommandListImmediate& RHICmdList)
		{
			UseComputeShader_RenderThread
			(
				RHICmdList,
				TextureRenderTargetResource,
				TextureRenderTargetName, 
				ShaderStructData,
				FeatureLevel
			);
		});

}


#undef LOCTEXT_NAMESPACE  