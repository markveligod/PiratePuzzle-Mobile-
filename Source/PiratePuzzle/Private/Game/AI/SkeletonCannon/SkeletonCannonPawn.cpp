// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonCannon/SkeletonCannonPawn.h"
#include "DrawDebugHelpers.h"
#include "Game/AI/SkeletonCannon/BulletActor.h"
#include "Game/AI/SkeletonCannon/Notifys/SpawnBulletAnimNotify.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSkeletonCannonPawn, All, All);

// Sets default values
ASkeletonCannonPawn::ASkeletonCannonPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create root scene component
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
    SetRootComponent(this->RootScene);

    // Create Skeletal Cannon
    this->SkeletalCannon = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Cannon");
    this->SkeletalCannon->SetupAttachment(this->RootScene);

    // Scene component for spawn bullet actor
    this->SceneSpawnBullet = CreateDefaultSubobject<USceneComponent>("Scene spawn bullet component");
    this->SceneSpawnBullet->SetupAttachment(this->RootScene);
}

// Called when the game starts or when spawned
void ASkeletonCannonPawn::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->SkeletalCannon, TEXT("Skeletal Cannon is nullptr"));

    this->TimeRateFire /= this->AnimShoot->RateScale;
    this->TimeRateReload /= this->AnimReload->RateScale;
    this->StateSkeletonCannon = EStateSkeletonCannon::Reload;

    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, this->TimeRateReload, false);

    const auto ListNotifyes = this->AnimShoot->Notifies;
    for (auto TempClassNotify : ListNotifyes)
    {
        auto CastNotify = Cast<USpawnBulletAnimNotify>(TempClassNotify.Notify);
        if (CastNotify)
        {
            CastNotify->OnSpawnBulletAnimNotify.AddUObject(this, &ASkeletonCannonPawn::SpawnBulletFromNotify);
        }
    }
}

void ASkeletonCannonPawn::SwapAnimState()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerAnimFR);
    this->StateSkeletonCannon =
        (this->StateSkeletonCannon == EStateSkeletonCannon::Fire) ? EStateSkeletonCannon::Reload : EStateSkeletonCannon::Fire;
    UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Swap state to %s"), *UEnum::GetValueAsString(this->StateSkeletonCannon));
    float TempRate = (this->StateSkeletonCannon == EStateSkeletonCannon::Fire) ? this->TimeRateFire : this->TimeRateReload;
    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, TempRate, false);
}

void ASkeletonCannonPawn::SpawnBulletFromNotify()
{
    FVector StartPos = this->SceneSpawnBullet->GetComponentLocation();
    FVector EndPos = StartPos + this->RootScene->GetRelativeRotation().Vector() * this->DistanceTrace;

    if (this->bEnableDebugTrace)
        DrawDebugLine(GetWorld(), StartPos, EndPos, this->ColorTrace, false, this->LifeTimeTrace, 0, this->RateThickness);

    const FVector Direction = (EndPos - StartPos).GetSafeNormal();

    FTransform SpawnTransform;
    SpawnTransform.SetLocation(StartPos);
    SpawnTransform.SetRotation(FQuat(FRotator::ZeroRotator));
    SpawnTransform.SetScale3D(FVector(1.f));
    ABulletActor* TempBullet = GetWorld()->SpawnActorDeferred<ABulletActor>(this->RefBulletClass, SpawnTransform);
    if (TempBullet)
    {
        TempBullet->SetShotDirection(Direction);
        TempBullet->FinishSpawning(SpawnTransform);
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->EffectShot, this->SceneSpawnBullet->GetComponentLocation());
        UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Skeleton Cannon: %s | Bullet: %s | Location: %s | is spawning"), *GetName(),
            *TempBullet->GetName(), *SpawnTransform.GetLocation().ToString());
    }
}
