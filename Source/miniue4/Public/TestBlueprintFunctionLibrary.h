// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestBlueprintFunctionLibrary.generated.h"


USTRUCT(Blueprintable)
struct FMyStructTest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Value;
};

USTRUCT(BlueprintInternalUseOnly)
struct FDummyStruct
{
	GENERATED_USTRUCT_BODY()
};


#define P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty)                               \
	Stack.MostRecentProperty = nullptr;                                             \
	Stack.StepCompiledIn<UArrayProperty>(NULL);                                     \
	void* ArrayAddr = Stack.MostRecentPropertyAddress;                              \
	UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty); \
	if (!ArrayProperty)                                                             \
	{                                                                               \
		Stack.bArrayContextFailed = true;                                           \
		return;                                                                     \
	}

/**
 * 
 */
UCLASS()
class MINIUE4_API UTestBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static void SayHello_Internal();

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static void SaySomething_Internal(const TArray<FString>& InWords);

	UFUNCTION(BlueprintCallable)
	static int Sum(int a, int b);

	UFUNCTION(BlueprintCallable, CustomThunk)
	static int SumCustom(int a, int b);

	DECLARE_FUNCTION(execSumCustom)
	{
		P_GET_PROPERTY(UIntProperty, Z_Param_a);
		P_GET_PROPERTY(UIntProperty, Z_Param_b);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*) Z_Param__Result = UTestBlueprintFunctionLibrary::Sum(Z_Param_a, Z_Param_b);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "MyDemo", meta = (CustomStructureParam = "CustomStruct"))
	static void ShowStructFields(const FDummyStruct& CustomStruct);

	static void Generic_ShowStructFields(const void* StructAddr, const UStructProperty* StructProperty);

	DECLARE_FUNCTION(execShowStructFields)
	{
		Stack.MostRecentPropertyAddress = nullptr;
		Stack.MostRecentProperty = nullptr;

		Stack.StepCompiledIn<UStructProperty>(NULL);
		void* StructAddr = Stack.MostRecentPropertyAddress;
		UStructProperty* StructProperty = Cast<UStructProperty>(Stack.MostRecentProperty);

		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_ShowStructFields(StructAddr, StructProperty);
		P_NATIVE_END;
	}

	UFUNCTION(BlueprintPure, CustomThunk,
		meta = (DisplayName = "Array Numeric Property Average", ArrayParm = "TargetArray",
			ArrayTypeDependentParams = "TargetArray"),
		Category = "MyDemo")
	static float Array_NumericPropertyAverage(const TArray<int32>& TargetArray, FName PropertyName);

	static float GenericArray_NumericPropertyAverage(
		const void* TargetArray, const UArrayProperty* ArrayProperty, FName ArrayPropertyName);


	DECLARE_FUNCTION(execArray_NumericPropertyAverage)
	{
		// get TargetArray
		P_GET_GENERIC_ARRAY(ArrayAddr, ArrayProperty);

		// get PropertyName
		P_GET_PROPERTY(UNameProperty, PropertyName);

		P_FINISH;

		P_NATIVE_BEGIN;
		*(float*) RESULT_PARAM = GenericArray_NumericPropertyAverage(ArrayAddr, ArrayProperty, PropertyName);
		P_NATIVE_END;
	}
};
