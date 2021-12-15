// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonCannon/BulletActor.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBulletActor, All, All);

// Sets default values
ABulletActor::ABulletActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create Root scene
    this->SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
    SetRootComponent(this->SceneRoot);

    // Create Static mesh
    this->StaticMeshBullet = CreateDefaultSubobject<UStaticMeshComponent>("Static mesh Bullet");
    this->StaticMeshBullet->SetupAttachment(this->SceneRoot);

    // Create projectile movement component
    this->ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement component");
    this->ProjectileMovement->ProjectileGravityScale = 0.0f;
    this->ProjectileMovement->InitialSpeed = 500.0f;

    // Create effect projectile
    this->EffectProjectile = CreateDefaultSubobject<UNiagaraComponent>("Effect projectile");
    this->EffectProjectile->SetupAttachment(this->SceneRoot);
}

void ABulletActor::SetShotDirection(FVector NormalDirection)
{
    this->ProjectileMovement->Velocity = NormalDirection * this->ProjectileMovement->InitialSpeed;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->SceneRoot, TEXT("Scene root is nullptr"));
    checkf(this->StaticMeshBullet, TEXT("Static Mesh Bullet"));
    checkf(this->ProjectileMovement, TEXT("Projectile movement component is nullptr"));

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    SetLifeSpan(this->RateLifeTimeBullet);
}

void ABulletActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (!OtherActor)
    {
        UE_LOG(LogBulletActor, Warning, TEXT("Call Register without param OtherActor"));
        return;
    }
    UE_LOG(LogBulletActor, Display, TEXT("Bullet: %s overlap other actor: %s"), *GetName(), *OtherActor->GetName());

    if (OtherActor->IsA(APiratePawn::StaticClass()))
    {
        APiratePawn* AIPirate = Cast<APiratePawn>(OtherActor);
        AIPirate->StopMovement();
        AIPirate->ChangeStateBrain(EStateBrain::FellCannon);
        this->GameMode->OnChangeGameStateTimer(EGameState::GameOver, 1.f);
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->EffectDestroy, GetActorLocation());
    Destroy();
}
