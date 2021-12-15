// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/PawnBase.h"
#include "SkeletonRunnerPawn.generated.h"

class USphereComponent;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API ASkeletonRunnerPawn : public APawnBase
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASkeletonRunnerPawn();

    // Getting current position on grid
    FIntPoint GetPositionSkeleton() const { return (this->SkeletonPosition); }

    virtual void StopMovement() override;

    // Go Go Go runner to Next point
    UFUNCTION(BlueprintCallable, Category = "ASkeletonRunnerPawn", meta = (ToolTip = "Go Go Go runner to Next point."))
    void GoToNextPoint();

    // Go Go Go runner to Current point
    void GoToCurrentPoint();

    // Go Go Go runner to Prev point
    void GoToPrevPoint();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Static mash component for displaying the attached stone
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* StoneMesh;
    // The name of the socket on the skeleton grid to attach to
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Runner",
        meta = (AllowPrivateAccess = true, ToolTip = "The name of the socket on the skeleton grid to attach to."))
    FName StoneNameSocket = "RockSocket";
    // The challenge interval is game over
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Runner",
        meta = (AllowPrivateAccess = true, ToolTip = "The challenge interval is game over."))
    float RateGameOverTimer = 1.f;

    // Range wait after stop movement
    FIntervalFloat RangeWaitMove;

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

    // Get next location from ArrayPointLocation
    FVector GetNextLocation();

    // Get next location from ArrayPointLocation
    FVector GetPrevLocation();

    // calculation of the next position in the array
    void CalculateNextCount();

    // calculation of the prev position in the array
    void CalculatePrevCount();

    friend class AGridGeneratorActor;
};
