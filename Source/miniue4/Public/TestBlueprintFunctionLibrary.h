// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MINIUE4_API UTestBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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
};
