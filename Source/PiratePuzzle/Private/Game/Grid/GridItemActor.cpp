// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridItemActor.h"
#include "Components/TextRenderComponent.h"

// Sets default values
AGridItemActor::AGridItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // Create Scene Static Mesh for group
    this->SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
    SetRootComponent(this->SceneRoot);

    // Create Static Mesh component
    this->StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    this->StaticMeshComponent->SetupAttachment(this->SceneRoot);

    // Create Debug Text Render
    this->TextDebugPosition = CreateDefaultSubobject<UTextRenderComponent>("Text Debug");
    this->TextDebugPosition->SetupAttachment(this->SceneRoot);
}

// Called when the game starts or when spawned
void AGridItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

