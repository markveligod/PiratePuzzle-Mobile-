// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridTreasurePlatform.h"
#include "Components/BoxComponent.h"

AGridTreasurePlatform::AGridTreasurePlatform()
{
    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());
}

void AGridTreasurePlatform::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->BoxCollision, TEXT("Box collision component is nullptr"));
}
