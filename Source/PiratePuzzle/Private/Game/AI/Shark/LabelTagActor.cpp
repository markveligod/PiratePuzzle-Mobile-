// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Shark/LabelTagActor.h"

// Sets default values
ALabelTagActor::ALabelTagActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create root scene component
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root scene component");
    SetRootComponent(this->RootScene);

    // Create debug widget component
    this->DebugMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Debug widget component");
    this->DebugMeshComponent->SetupAttachment(this->RootScene);
}

// Called when the game starts or when spawned
void ALabelTagActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->DebugMeshComponent, TEXT("Debug Mesh is nullptr"));

    // Delete debug widget
    this->DebugMeshComponent->DestroyComponent();
}