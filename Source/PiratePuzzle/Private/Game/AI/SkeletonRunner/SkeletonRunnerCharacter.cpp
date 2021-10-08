// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "Components/SphereComponent.h"
#include "Game/GamePlayMode.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/Grid/GridPlatformActor.h"

// Sets default values
ASkeletonRunnerCharacter::ASkeletonRunnerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create collision attack
    this->SphereAttackCollision = CreateDefaultSubobject<USphereComponent>("Sphere component");
    this->SphereAttackCollision->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASkeletonRunnerCharacter::BeginPlay()
{
    Super::BeginPlay();
    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->StateSkeletonRunner = EStateAI::Walk;
}

void ASkeletonRunnerCharacter::OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

FVector ASkeletonRunnerCharacter::GetNextLocation()
{
    FIntPoint TempPoint;
    TempPoint = this->ArrayPointLocation[this->CountLoc];
    (this->bIsReverseArray) ? this->CountLoc-- : this->CountLoc++;
    if (this->CountLoc == -1)
    {
        this->bIsReverseArray = false;
        this->CountLoc = 0;
    }
    if (this->CountLoc == this->ArrayPointLocation.Num())
    {
        this->bIsReverseArray = true;
        this->CountLoc--;
    }
    this->SkeletonPosition = TempPoint;
    FVector TempLocation = this->GameMode->GetGridGenerator()->GetMapPlatformOnGrid()[TempPoint]->GetActorLocation();
    return (TempLocation);
}
