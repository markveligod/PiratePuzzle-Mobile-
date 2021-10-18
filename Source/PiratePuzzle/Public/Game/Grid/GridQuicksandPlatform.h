// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/Grid/GridPlatformActor.h"
#include "GridQuicksandPlatform.generated.h"

class UNiagaraComponent;
class APirateAICharacter;
class UBoxComponent;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGridQuicksandPlatform : public AGridPlatformActor
{
    GENERATED_BODY()

public:
    AGridQuicksandPlatform();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    // Box collision for register begin overlap with Pirate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;
    // Effect for emulating quicksand
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UNiagaraComponent* EffectQuickSand;

    // Time of immersion in sand
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time of immersion in sand."))
    float RateTimeSand = 1.f;
    // To what depth to immerse on axis Z
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "To what depth to immerse on axis Z."))
    float Depth = 100.f;

    AActor* SuicideBomber;
    bool bEnableDepth = false;
    float TimeElyps = 0.f;
    float StartPos;
    float EndPos;

    // Pirate dive function
    void DiveAIPirate(float DeltaTime);

    // Register begin overlap
    UFUNCTION()
    void RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
