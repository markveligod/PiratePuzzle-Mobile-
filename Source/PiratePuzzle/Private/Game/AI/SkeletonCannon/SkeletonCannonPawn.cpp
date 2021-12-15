// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonCannon/SkeletonCannonPawn.h"
#include "DrawDebugHelpers.h"
#include "Game/AI/SkeletonCannon/BulletActor.h"
#include "Game/AI/SkeletonCannon/Notifys/SpawnBulletAnimNotify.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "PPGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkeletonCannonPawn, All, All);

// Sets default values
ASkeletonCannonPawn::ASkeletonCannonPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Scene component for spawn bullet actor
    this->SceneSpawnBullet = CreateDefaultSubobject<USceneComponent>("Scene spawn bullet component");
    this->SceneSpawnBullet->SetupAttachment(GetRootComponent());
}

void ASkeletonCannonPawn::StopFireCannon()
{
    ChangeStateBrain(EStateBrain::Idle);
    GetWorldTimerManager().ClearTimer(this->TimerAnimFR);
}

void ASkeletonCannonPawn::StartFireCannon()
{
    ChangeStateBrain(EStateBrain::Reload);
    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, this->TimeRateReload, false);
}

// Called when the game starts or when spawned
void ASkeletonCannonPawn::BeginPlay()
{
    Super::BeginPlay();

    // Spawn of the required number of cores
    const FVector StartPos = this->SceneSpawnBullet->GetComponentLocation();
    const FVector EndPos = StartPos + GetRootComponent()->GetRelativeRotation().Vector() * this->DistanceTrace;

    if (this->bEnableDebugInfo)
        DrawDebugLine(GetWorld(), StartPos, EndPos, this->ColorTrace, false, this->TimeLifeTrace, 0, this->ThicknessTrace);

    this->DirectionShot = (EndPos - StartPos).GetSafeNormal();

    // Settings for calling swap animations
    this->TimeRateFire /= this->AnimShoot->RateScale;
    this->TimeRateReload /= this->AnimReload->RateScale;

    // Caption for the animated shot notification
    const auto ListNotifies = this->AnimShoot->Notifies;
    for (const auto TempClassNotify : ListNotifies)
    {
        const auto CastNotify = Cast<USpawnBulletAnimNotify>(TempClassNotify.Notify);
        if (CastNotify)
        {
            UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Name skeleton: %s | Name notify to cast: %s"), *GetName(), *CastNotify->GetName());
            CastNotify->OnSpawnBulletAnimNotify.AddUObject(this, &ASkeletonCannonPawn::SpawnBulletFromNotify);
        }
    }

    // Test Spawning cannon
    if (GetPPGameInstance()->GetRunLevel() == 28) this->SpawnBulletFromNotify();
}

void ASkeletonCannonPawn::SwapAnimState()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerAnimFR);

    ChangeStateBrain((GetStateBrain() == EStateBrain::Fire) ? EStateBrain::Reload : EStateBrain::Fire);
    UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Name skeleton: %s | Swap state to %s"), *GetName(),
        *UEnum::GetValueAsString(GetStateBrain()));

    const float TempRate = (GetStateBrain() == EStateBrain::Fire) ? this->TimeRateFire : this->TimeRateReload;
    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, TempRate, false);
}

void ASkeletonCannonPawn::SpawnBulletFromNotify()
{
    FTransform TempTransform;
    TempTransform.SetLocation(this->SceneSpawnBullet->GetComponentLocation());
    TempTransform.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));
    TempTransform.SetScale3D(FVector(1.0f));

    ABulletActor* TempBullet = GetWorld()->SpawnActorDeferred<ABulletActor>(this->RefBulletClass, TempTransform);
    if (!TempBullet)
    {
        UE_LOG(LogSkeletonCannonPawn, Error, TEXT("Skeleton cannon: %s | spawn bullet is nullptr"), *GetName());
        return;
    }

    TempBullet->SetActorLocation(this->SceneSpawnBullet->GetComponentLocation());
    TempBullet->SetShotDirection(this->DirectionShot);
    TempBullet->FinishSpawning(TempTransform);

    UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Skeleton Cannon: %s | Bullet: %s | Location: %s | is spawning"), *GetName(),
        *TempBullet->GetName(), *TempBullet->GetActorLocation().ToString());
}
