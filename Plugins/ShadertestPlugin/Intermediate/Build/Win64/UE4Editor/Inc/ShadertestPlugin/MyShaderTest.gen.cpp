// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ShadertestPlugin/Public/MyShaderTest.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMyShaderTest() {}
// Cross Module References
	SHADERTESTPLUGIN_API UClass* Z_Construct_UClass_UTestShaderBlueprintLibrary_NoRegister();
	SHADERTESTPLUGIN_API UClass* Z_Construct_UClass_UTestShaderBlueprintLibrary();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UPackage* Z_Construct_UPackage__Script_ShadertestPlugin();
	SHADERTESTPLUGIN_API UFunction* Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FLinearColor();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTextureRenderTarget2D_NoRegister();
// End Cross Module References
	void UTestShaderBlueprintLibrary::StaticRegisterNativesUTestShaderBlueprintLibrary()
	{
		UClass* Class = UTestShaderBlueprintLibrary::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "DrawTestShaderRenderTarget", &UTestShaderBlueprintLibrary::execDrawTestShaderRenderTarget },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics
	{
		struct TestShaderBlueprintLibrary_eventDrawTestShaderRenderTarget_Parms
		{
			UTextureRenderTarget2D* OutputRenderTarget;
			AActor* Ac;
			FLinearColor MyColor;
		};
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_MyColor;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Ac;
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_OutputRenderTarget;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_MyColor = { "MyColor", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TestShaderBlueprintLibrary_eventDrawTestShaderRenderTarget_Parms, MyColor), Z_Construct_UScriptStruct_FLinearColor, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_Ac = { "Ac", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TestShaderBlueprintLibrary_eventDrawTestShaderRenderTarget_Parms, Ac), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_OutputRenderTarget = { "OutputRenderTarget", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(TestShaderBlueprintLibrary_eventDrawTestShaderRenderTarget_Parms, OutputRenderTarget), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_MyColor,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_Ac,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::NewProp_OutputRenderTarget,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::Function_MetaDataParams[] = {
		{ "Category", "ShaderTestPlugin" },
		{ "ModuleRelativePath", "Public/MyShaderTest.h" },
		{ "WorldContext", "WorldCongtextObject" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTestShaderBlueprintLibrary, nullptr, "DrawTestShaderRenderTarget", sizeof(TestShaderBlueprintLibrary_eventDrawTestShaderRenderTarget_Parms), Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UTestShaderBlueprintLibrary_NoRegister()
	{
		return UTestShaderBlueprintLibrary::StaticClass();
	}
	struct Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_ShadertestPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTestShaderBlueprintLibrary_DrawTestShaderRenderTarget, "DrawTestShaderRenderTarget" }, // 767942774
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "MyShaderTest.h" },
		{ "ModuleRelativePath", "Public/MyShaderTest.h" },
		{ "ScriptName", "TestShaderLibrary" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTestShaderBlueprintLibrary>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::ClassParams = {
		&UTestShaderBlueprintLibrary::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x000800A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTestShaderBlueprintLibrary()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UTestShaderBlueprintLibrary, 1757973961);
	template<> SHADERTESTPLUGIN_API UClass* StaticClass<UTestShaderBlueprintLibrary>()
	{
		return UTestShaderBlueprintLibrary::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UTestShaderBlueprintLibrary(Z_Construct_UClass_UTestShaderBlueprintLibrary, &UTestShaderBlueprintLibrary::StaticClass, TEXT("/Script/ShadertestPlugin"), TEXT("UTestShaderBlueprintLibrary"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTestShaderBlueprintLibrary);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
