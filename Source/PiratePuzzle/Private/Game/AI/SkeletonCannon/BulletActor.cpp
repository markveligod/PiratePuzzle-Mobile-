// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonCannon/BulletActor.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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

    // Create effect projectile
    this->EffectProjectile = CreateDefaultSubobject<UNiagaraComponent>("Effect projectile");
    this->EffectProjectile->SetupAttachment(this->SceneRoot);
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->SceneRoot, TEXT("Scene root is nullptr"));
    checkf(this->StaticMeshBullet, TEXT("Static Mesh Bullet"));
    checkf(this->ProjectileMovement, TEXT("Projectile movement component is nullptr"));
    this->StaticMeshBullet->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnRegisterCollisionBeginOverlap);
    this->ProjectileMovement->Velocity = this->DirectionBullet * this->ProjectileMovement->InitialSpeed;
    SetLifeSpan(this->RateLifeTimeBullet);

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
}

void ABulletActor::OnRegisterCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogBulletActor, Warning, TEXT("Call Register without param OtherActor"));
        return;
    }
    UE_LOG(LogBulletActor, Display, TEXT("Bullet: %s overlap other actor: %s"), *GetName(), *OtherActor->GetName());

    if (OtherActor->IsA(APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* AIPirate = Cast<APirateAICharacter>(OtherActor);
        AIPirate->SetStateAI(EStateAI::Death);
        AIPirate->GetCharacterMovement()->StopActiveMovement();
        this->GameMode->OnChangeGameStateTimer(EGameState::GameOver);
    }
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->EffectDestroy, GetActorLocation());
    this->StaticMeshBullet->SetVisibility(false);
    this->ProjectileMovement->Velocity = FVector::ZeroVector;
    SetLifeSpan(0.5f);
}
