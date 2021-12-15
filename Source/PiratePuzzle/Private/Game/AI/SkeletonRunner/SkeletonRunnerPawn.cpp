// Is the property of WerFEST Software

#include "Game/AI/SkeletonRunner/SkeletonRunnerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/Grid/GridPlatformActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkeletonRunnerPawn, All, All);

ASkeletonRunnerPawn::ASkeletonRunnerPawn()
{
    // Create Stone mesh
    this->StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("Stone Mesh");
    this->StoneMesh->SetupAttachment(GetMesh(), this->StoneNameSocket);
}

void ASkeletonRunnerPawn::StopMovement()
{
    Super::StopMovement();
    if (GetStateBrain() == EStateBrain::Idle && GetGamePlayMode()->GetGameState() == EGameState::GameProgress) this->GoToNextPoint();
}

void ASkeletonRunnerPawn::GoToNextPoint()
{
    FVector NextLoc = this->GetNextLocation();
    NextLoc.Z = GetActorLocation().Z;
    const float RateTime = UKismetMathLibrary::RandomFloatInRange(this->RangeWaitMove.Min, this->RangeWaitMove.Max);
    StartRotateTimer(GetActorRotation(), CalculatePawnRotation(GetActorLocation(), NextLoc), RateTime - 1.f);
    StartMovementTimer(GetActorLocation(), NextLoc, RateTime);
}

void ASkeletonRunnerPawn::GoToCurrentPoint()
{
    auto MapPlatform = GetGamePlayMode()->GetGridGenerator()->GetMapPlatformOnGrid();
    FVector NextLoc = BaseUtils::GetVectorPositionPlatform(this->SkeletonPosition, MapPlatform);
    NextLoc.Z = GetActorLocation().Z;
    const float RateTime = UKismetMathLibrary::RandomFloatInRange(this->RangeWaitMove.Min, this->RangeWaitMove.Max);
    StartRotateTimer(GetActorRotation(), CalculatePawnRotation(GetActorLocation(), NextLoc), RateTime - 1.f);
    StartMovementTimer(GetActorLocation(), NextLoc, RateTime);
}

void ASkeletonRunnerPawn::GoToPrevPoint()
{
    FVector NextLoc = this->GetPrevLocation();
    NextLoc.Z = GetActorLocation().Z;
    SetActorLocation(NextLoc);
    StopMovement();
}

void ASkeletonRunnerPawn::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->StoneMesh, TEXT("Stone mesh is nullptr"));

    GetCollision()->OnComponentBeginOverlap.AddDynamic(this, &ASkeletonRunnerPawn::OnRegisterOverlap);

    if (this->ArrayPointLocation.Num() > 0)
    {
        this->SkeletonPosition = this->ArrayPointLocation[this->CountLoc];
        this->CalculateNextCount();
    }
}

void ASkeletonRunnerPawn::OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogSkeletonRunnerPawn, Warning, TEXT("Other actor is nullptr"));
        return;
    }
    UE_LOG(LogSkeletonRunnerPawn, Display, TEXT("Skeleton runner: %s | Overlap Actor: %s"), *GetName(), *OtherActor->GetName());

    if (OtherActor->IsA(APiratePawn::StaticClass()))
    {
        // Dead Pirate
        APiratePawn* TempPirate = Cast<APiratePawn>(OtherActor);
        TempPirate->StopMovement();
        TempPirate->ChangeStateBrain(EStateBrain::FellSkeleton);
        const FVector DirectionToSkeleton = (GetActorLocation() - TempPirate->GetActorLocation()).GetSafeNormal();
        TempPirate->SetActorRotation(DirectionToSkeleton.Rotation());

        // Skeleton runner attack
        ChangeStateBrain(EStateBrain::Attack);
        StopMovement();
        const FVector DirectionToPirate = (TempPirate->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRotation(DirectionToPirate.Rotation());

        // Game Over
        GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameOver, this->RateGameOverTimer);
    }
}

FVector ASkeletonRunnerPawn::GetNextLocation()
{
    const FIntPoint TempPoint = this->ArrayPointLocation[this->CountLoc];
    this->CalculateNextCount();
    this->SkeletonPosition = TempPoint;
    const FVector TempLocation = GetGamePlayMode()->GetGridGenerator()->GetMapPlatformOnGrid()[TempPoint]->GetActorLocation();
    return (TempLocation);
}

FVector ASkeletonRunnerPawn::GetPrevLocation()
{
    this->CalculatePrevCount();
    const FIntPoint TempPoint = this->ArrayPointLocation[this->CountLoc];
    this->SkeletonPosition = TempPoint;
    const FVector TempLocation = GetGamePlayMode()->GetGridGenerator()->GetMapPlatformOnGrid()[TempPoint]->GetActorLocation();
    UE_LOG(LogSkeletonRunnerPawn, Display, TEXT("Name skeleton runner: %s | Calculate prev count: %i | New point pos: %s | Location: %s"),
        *GetName(), this->CountLoc, *TempPoint.ToString(), *TempLocation.ToString());
    return (TempLocation);
}

void ASkeletonRunnerPawn::CalculateNextCount()
{
    (this->bIsReverseArray) ? this->CountLoc-- : this->CountLoc++;
    if (this->CountLoc == -1)
    {
        this->bIsReverseArray = false;
        this->CountLoc = 1;
    }
    if (this->CountLoc == this->ArrayPointLocation.Num())
    {
        this->bIsReverseArray = true;
        this->CountLoc -= 2;
    }
}

void ASkeletonRunnerPawn::CalculatePrevCount()
{
    (this->bIsReverseArray) ? this->CountLoc++ : this->CountLoc--;
    if (this->CountLoc == -1)
    {
        this->CountLoc = 0;
    }
    if (this->CountLoc == this->ArrayPointLocation.Num())
    {
        this->CountLoc--;
    }
}
