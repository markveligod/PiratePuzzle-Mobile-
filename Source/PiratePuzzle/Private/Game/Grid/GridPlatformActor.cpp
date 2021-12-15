// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridPlatformActor.h"
#include "Components/TextRenderComponent.h"
#include "Game/GamePlayMode.h"

// Sets default values
AGridPlatformActor::AGridPlatformActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    // Create Scene Static Mesh for group
    this->SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
    SetRootComponent(this->SceneRoot);

    // Create Static Mesh component
    this->BaseMeshPlatform = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
    this->BaseMeshPlatform->SetupAttachment(this->SceneRoot);
    this->BaseMeshPlatform->SetCollisionObjectType(ECC_WorldStatic);
    this->BaseMeshPlatform->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    this->BaseMeshPlatform->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    // Create Debug Text Render
    this->TextDebugPosition = CreateDefaultSubobject<UTextRenderComponent>("Text Debug");
    this->TextDebugPosition->SetupAttachment(this->SceneRoot);
}

// Called when the game starts or when spawned
void AGridPlatformActor::BeginPlay()
{
    Super::BeginPlay();
    this->TextDebugPosition->DestroyComponent();
    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
}
