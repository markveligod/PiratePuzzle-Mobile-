// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridWallActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AGridWallActor::AGridWallActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create root scene
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene component");
    SetRootComponent(this->RootScene);

    // Create Box wall component
    this->WallBox = CreateDefaultSubobject<UBoxComponent>("Box wall component");
    this->WallBox->SetupAttachment(this->RootScene);
    this->WallBox->SetCollisionProfileName("BlockAll");
}

// Called when the game starts or when spawned
void AGridWallActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->WallBox, TEXT("Wall box component is nullptr"));
}