// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridGeneratorActor.h"

// Sets default values
AGridGeneratorActor::AGridGeneratorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create root component
    this->StaticMeshHandleComponent = CreateDefaultSubobject<UStaticMeshComponent>("Root component");
    SetRootComponent(this->StaticMeshHandleComponent);
}

// Called when the game starts or when spawned
void AGridGeneratorActor::BeginPlay()
{
	Super::BeginPlay();
	checkf(this->StaticMeshHandleComponent, TEXT("Root component is nullptr"));
}

void AGridGeneratorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    
}


