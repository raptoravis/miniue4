// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "MyTest.generated.h"

UENUM(BlueprintType)
enum class EMyEnum : uint8
{
	MY_Dance UMETA(DisplayName = "Dance"),
	MY_Rain UMETA(DisplayName = "Rain"),
	MY_Song UMETA(DisplayName = "Song")
};

USTRUCT(BlueprintType)
struct MINIUE4_API FMyStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Score;
};

UINTERFACE(BlueprintType, Blueprintable)
class MINIUE4_API UMyInterface : public UInterface
{
	GENERATED_BODY()
};
class IMyInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NativeInterfaceFunc();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ImplementableInterfaceFunc();
};

UCLASS(BlueprintType, Blueprintable)
class MINIUE4_API UMyClass : public UObject, public IMyInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	float Score;

public:
	UFUNCTION(BlueprintCallable, Category = "Hello")
	int32 Func(float param1);	// C++实现，蓝图调用

	UFUNCTION(BlueprintNativeEvent, Category = "Hello")
	void NativeFunc();	// C++实现默认版本，蓝图可重载实现

	UFUNCTION(BlueprintImplementableEvent, Category = "Hello")
	void ImplementableFunc();	// C++不实现，蓝图实现
};
