// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class MINIUE4_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	float Score;

public:
	UFUNCTION(BlueprintCallable, Category = "Hello")
	void CallableFunc();	// C++实现，蓝图调用

	UFUNCTION(BlueprintNativeEvent, Category = "Hello")
	void NativeFunc();	// C++实现默认版本，蓝图可重载实现

	UFUNCTION(BlueprintImplementableEvent, Category = "Hello")
	void ImplementableFunc();	// C++不实现，蓝图实现
};
