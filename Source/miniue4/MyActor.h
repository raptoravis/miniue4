// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

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
	void CallableFunc();	// C++ʵ�֣���ͼ����

	UFUNCTION(BlueprintNativeEvent, Category = "Hello")
	void NativeFunc();	// C++ʵ��Ĭ�ϰ汾����ͼ������ʵ��

	UFUNCTION(BlueprintImplementableEvent, Category = "Hello")
	void ImplementableFunc();	// C++��ʵ�֣���ͼʵ��
};
