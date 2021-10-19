// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Gold/GoldActor.h"

// Sets default values
AGoldActor::AGoldActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoldActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AGoldActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
