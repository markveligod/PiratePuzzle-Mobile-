// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/AIDataTypes.h"
#include "GameFramework/Pawn.h"
#include "SkeletonCannonPawn.generated.h"

class UNiagaraSystem;
class ABulletActor;
UCLASS()
class PIRATEPUZZLE_API ASkeletonCannonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkeletonCannonPawn();
    
    // Get State animation skeleton state
    UFUNCTION(BlueprintCallable)
    EStateSkeletonCannon GetStateSkeletonCannon() const { return (this->StateSkeletonCannon); }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    // Root Scene
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* RootScene;
    // Skeletal Mesh Cannon
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* SkeletalCannon;
    // Scene spawn bullet
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* SceneSpawnBullet;
    
    // Animation Fire playback time
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Animation Fire playback time."))
    float TimeRateFire = 1.f;
    // Animation Reload playback time
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Animation Reload playback time."))
    float TimeRateReload = 1.f;
    // Pointer to the animation of shooting
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Pointer to the animation of shooting."))
    UAnimSequence* AnimShoot = nullptr;
    // Pointer to the animation of Reload
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Animation Reload playback time."))
    UAnimSequence* AnimReload = nullptr;
    // Reference sub class bullet
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Reference sub class bullet."))
    TSubclassOf<ABulletActor> RefBulletClass;
    // Effect to simulate a cannon shot
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Pawn", meta = (AllowPrivateAccess = true, ToolTip = "Effect to simulate a cannon shot."))
    UNiagaraSystem* EffectShot;
    
    // Enable Debug trace bullet
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Bullet", meta = (AllowPrivateAccess = true, ToolTip = "Enable Debug trace bullet."))
    bool bEnableDebugTrace = true;
    // Check distance Trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Bullet", meta = (AllowPrivateAccess = true, ToolTip = "Check distance Trace.", EditCondition = "bEnableDebugTrace"))
    float DistanceTrace = 1000.f;
    // Color trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Bullet", meta = (AllowPrivateAccess = true, ToolTip = "Color trace.", EditCondition = "bEnableDebugTrace"))
    FColor ColorTrace = FColor::Red;
    // Life time trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Bullet", meta = (AllowPrivateAccess = true, ToolTip = "Color trace.", EditCondition = "bEnableDebugTrace"))
    float LifeTimeTrace = 5.f;
    // Thickness trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Bullet", meta = (AllowPrivateAccess = true, ToolTip = "Color trace.", EditCondition = "bEnableDebugTrace"))
    float RateThickness = 2.f;

    // State animation skeleton and cannon
    EStateSkeletonCannon StateSkeletonCannon = EStateSkeletonCannon::None;

    // Timer for looping animation fire and reload
    FTimerHandle TimerAnimFR;

    // Function Swap State Reload and Fire
    void SwapAnimState();
    
    // Function Spawn bullet
    void SpawnBulletFromNotify();
};
