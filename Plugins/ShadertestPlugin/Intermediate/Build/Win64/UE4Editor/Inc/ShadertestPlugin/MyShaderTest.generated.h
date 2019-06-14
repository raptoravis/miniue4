// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UTextureRenderTarget2D;
class AActor;
struct FLinearColor;
#ifdef SHADERTESTPLUGIN_MyShaderTest_generated_h
#error "MyShaderTest.generated.h already included, missing '#pragma once' in MyShaderTest.h"
#endif
#define SHADERTESTPLUGIN_MyShaderTest_generated_h

#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execDrawTestShaderRenderTarget) \
	{ \
		P_GET_OBJECT(UTextureRenderTarget2D,Z_Param_OutputRenderTarget); \
		P_GET_OBJECT(AActor,Z_Param_Ac); \
		P_GET_STRUCT(FLinearColor,Z_Param_MyColor); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UTestShaderBlueprintLibrary::DrawTestShaderRenderTarget(Z_Param_OutputRenderTarget,Z_Param_Ac,Z_Param_MyColor); \
		P_NATIVE_END; \
	}


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execDrawTestShaderRenderTarget) \
	{ \
		P_GET_OBJECT(UTextureRenderTarget2D,Z_Param_OutputRenderTarget); \
		P_GET_OBJECT(AActor,Z_Param_Ac); \
		P_GET_STRUCT(FLinearColor,Z_Param_MyColor); \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		UTestShaderBlueprintLibrary::DrawTestShaderRenderTarget(Z_Param_OutputRenderTarget,Z_Param_Ac,Z_Param_MyColor); \
		P_NATIVE_END; \
	}


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUTestShaderBlueprintLibrary(); \
	friend struct Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics; \
public: \
	DECLARE_CLASS(UTestShaderBlueprintLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ShadertestPlugin"), SHADERTESTPLUGIN_API) \
	DECLARE_SERIALIZER(UTestShaderBlueprintLibrary)


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_INCLASS \
private: \
	static void StaticRegisterNativesUTestShaderBlueprintLibrary(); \
	friend struct Z_Construct_UClass_UTestShaderBlueprintLibrary_Statics; \
public: \
	DECLARE_CLASS(UTestShaderBlueprintLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/ShadertestPlugin"), SHADERTESTPLUGIN_API) \
	DECLARE_SERIALIZER(UTestShaderBlueprintLibrary)


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTestShaderBlueprintLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(SHADERTESTPLUGIN_API, UTestShaderBlueprintLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTestShaderBlueprintLibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(UTestShaderBlueprintLibrary&&); \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(const UTestShaderBlueprintLibrary&); \
public:


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(UTestShaderBlueprintLibrary&&); \
	SHADERTESTPLUGIN_API UTestShaderBlueprintLibrary(const UTestShaderBlueprintLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(SHADERTESTPLUGIN_API, UTestShaderBlueprintLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UTestShaderBlueprintLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UTestShaderBlueprintLibrary)


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_PRIVATE_PROPERTY_OFFSET
#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_9_PROLOG
#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_PRIVATE_PROPERTY_OFFSET \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_RPC_WRAPPERS \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_INCLASS \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_PRIVATE_PROPERTY_OFFSET \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_INCLASS_NO_PURE_DECLS \
	miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h_12_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class TestShaderBlueprintLibrary."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SHADERTESTPLUGIN_API UClass* StaticClass<class UTestShaderBlueprintLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID miniue4_Plugins_ShadertestPlugin_Source_ShadertestPlugin_Public_MyShaderTest_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
