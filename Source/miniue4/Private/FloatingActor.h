#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleGameCore/Public/CFloatingActor.h"
#include "UnrealActorAdapter.h"
#include "UnrealMathAdapter.h"
#include "FloatingActor.generated.h"
 
UCLASS()
class MINIUE4_API AFloatingActor : public AActor
{
    GENERATED_BODY()
 
private:    
    TSharedPtr<IUnrealMath> mMathAdapter;
    TSharedPtr<IUnrealActor> mActorAdapter;
    TSharedPtr<CFloatingActor> mCFloatingActor;
     
public: 
    // Sets default values for this actor's properties
    AFloatingActor();
 
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
 
public: 
    // Called every frame
    virtual void Tick(float DeltaTime) override;
 
};
