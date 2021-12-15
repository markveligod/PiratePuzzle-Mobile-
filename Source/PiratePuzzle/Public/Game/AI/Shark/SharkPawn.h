// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/PawnBase.h"
#include "SharkPawn.generated.h"

class ALabelTagActor;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API ASharkPawn : public APawnBase
{
    GENERATED_BODY()

public:
    virtual void StopMovement() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Constructor move position
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // A reference for creating a pointer to the location of the next location.
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "A reference for creating a pointer to the location of the next location."))
    TSubclassOf<ALabelTagActor> RefLabelTag;
    // Array of positions on which the shark will move
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "Array of positions on which the shark will move."))
    TArray<FVector> ArrayPosTags;
    // Time pause movement to the next point
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time pause movement to the next point."))
    FIntervalFloat RangeMove{2.0f, 5.0f};

    // Current count position in Array
    int32 PosCountTag = 0;

    // Array tags
    TArray<ALabelTagActor*> ArrayTags;

    // Function clear tags
    void ClearTags();

    // Reset Default position
    void ResetArrayPosition();

    // Function Spawn tag on scene
    void SpawnTags();

    // Get next location tag
    FVector GetNextPosTag();

    // Go go go shark
    void GoToNextPosTag();
};
