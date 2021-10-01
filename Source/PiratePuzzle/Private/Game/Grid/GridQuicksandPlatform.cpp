// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridQuicksandPlatform.h"
#include "Components/BoxComponent.h"

AGridQuicksandPlatform::AGridQuicksandPlatform()
{
    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());
}

void AGridQuicksandPlatform::BeginPlay()
{
    Super::BeginPlay();
    
}
