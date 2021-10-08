// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/AI/AIDataTypes.h"
#include "SkeletonRunnerCharacter.generated.h"

class AGamePlayMode;
class USphereComponent;
class UBehaviorTree;
UCLASS()
class PIRATEPUZZLE_API ASkeletonRunnerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASkeletonRunnerCharacter();

    // Pointer on Behavior Tree
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree")
    UBehaviorTree* BehaviorTreeAsset;

    // Get next location from ArrayPointLocation
    FVector GetNextLocation();

    // Get and Set New State Ai skeleton runner
    UFUNCTION(BlueprintCallable)
    const EStateAI GetStateAISkeletonRunner() const { return (this->StateSkeletonRunner); }
    void SetNewStateAISkeletonRunner(EStateAI NewState) { this->StateSkeletonRunner = NewState; }

    // Getting current position on grid
    FIntPoint GetPositionSkeleton() const { return (this->SkeletonPosition); }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USphereComponent* SphereAttackCollision;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    EStateAI StateSkeletonRunner = EStateAI::None;
    AGamePlayMode* GameMode;

    // Overlap register
    UFUNCTION()
    void OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // State is Revers from ArrayPointLocation
    bool bIsReverseArray = false;
    // Count for define next location
    int32 CountLoc = 0;
    // Array point position for road route
    TArray<FIntPoint> ArrayPointLocation;
    // Current position on Grid
    FIntPoint SkeletonPosition;

    friend class AGridGeneratorActor;
};
