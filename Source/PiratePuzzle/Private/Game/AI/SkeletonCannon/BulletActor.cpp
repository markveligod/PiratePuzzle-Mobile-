// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/AI/SkeletonCannon/BulletActor.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
}

void ABulletActor::OnRegisterCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor) return;

    UE_LOG(LogBulletActor, Display, TEXT("Bullet: %s overlap other actor: %s"), *GetName(), *OtherActor->GetName());
    Destroy();
}

