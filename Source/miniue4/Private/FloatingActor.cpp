#include "FloatingActor.h"
 
// Sets default values
AFloatingActor::AFloatingActor()
{
    mMathAdapter = TSharedPtr<IUnrealMath>(new UnrealMathAdapter());
    mActorAdapter = TSharedPtr<IUnrealActor>(new UnrealActorAdapter(*this));
    mCFloatingActor = TSharedPtr<CFloatingActor>(new CFloatingActor(*mActorAdapter, *mMathAdapter));    
 
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}
 
// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
    Super::BeginPlay();
     
}
 
// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
 
    mCFloatingActor->Tick(DeltaTime);
}
