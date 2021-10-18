// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

class UNiagaraComponent;
class AGamePlayMode;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;
UCLASS()
class PIRATEPUZZLE_API ABulletActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABulletActor();

    // Set new direction
    void SetShotDirection(FVector NormalDirection) { this->DirectionBullet = NormalDirection; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Pointer on Current Game play mode
    AGamePlayMode* GameMode;

    // Direction variable
    FVector DirectionBullet;
    // Root Scene
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* SceneRoot;
    // Static mesh bullet
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* StaticMeshBullet;
    // Projectile Movement
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UProjectileMovementComponent* ProjectileMovement;
    // Projectile effect
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UNiagaraComponent* EffectProjectile;

    // Rate life time bullet
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings spawn",
        meta = (AllowPrivateAccess = true, ToolTip = "Rate life time bullet"))
    float RateLifeTimeBullet = 5.f;
    // The effect after the destruction of the core in a collision
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings spawn",
        meta = (AllowPrivateAccess = true, ToolTip = "The effect after the destruction of the core in a collision"))
    UNiagaraSystem* EffectDestroy;

    // Register overlap with pirate
    UFUNCTION()
    void OnRegisterCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
