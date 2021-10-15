// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "Components/SphereComponent.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerAIController.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/Grid/GridPlatformActor.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkeletonRunner, All, All);

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
    this->SphereAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonRunnerCharacter::OnRegisterOverlap);
}

void ASkeletonRunnerCharacter::OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogSkeletonRunner, Warning, TEXT("Other actor is nullptr"));
        return;
    }
    UE_LOG(LogSkeletonRunner, Display, TEXT("Skeleton runner: %s | Overlap Actor: %s"), *GetName(), *OtherActor->GetName());

    if (OtherActor->IsA(APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* TempPirate = Cast<APirateAICharacter>(OtherActor);
        TempPirate->SetStateAI(EStateAI::Death);
        TempPirate->GetCharacterMovement()->StopActiveMovement();
        FVector DirectionToSkeleton = (GetActorLocation() - TempPirate->GetActorLocation()).GetSafeNormal();
        TempPirate->SetActorRelativeRotation(DirectionToSkeleton.ToOrientationRotator());

        GetCharacterMovement()->StopActiveMovement();
        FVector DirectionToPirate = (TempPirate->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRelativeRotation(DirectionToPirate.ToOrientationRotator());
        this->StateSkeletonRunner = EStateAI::Attack;
        this->GameMode->OnChangeGameStateTimer(EGameState::GameOver);
    }
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
