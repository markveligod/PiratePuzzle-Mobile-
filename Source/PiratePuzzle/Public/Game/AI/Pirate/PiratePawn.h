// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/PawnBase.h"
#include "PiratePawn.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API APiratePawn : public APawnBase
{
    GENERATED_BODY()

public:
    // Setting New point position on grid
    void SetNewPoint(const FIntPoint& NewPoint) { this->PiratePointGrid = NewPoint; }

    // Getting the previous position on the grid or return Error (FIntPoint(-1, -1))
    FIntPoint GetPreviousPoint();

    // Getting current point position on grid
    FIntPoint GetPointPosition() const { return (this->PiratePointGrid); }

    // Adding a position to an ArrayPointPosition
    void AddPreviousPointPosition(const FIntPoint& NewPoint) { this->ArrayPointPosition.Add(NewPoint); }

    // Signature on changes to the state of the pirate's brains
    FOnStateBrainChangedSignature OnStateBrainChanged;

    virtual void ChangeStateBrain(EStateBrain NewState) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Position Player on grid
    FIntPoint PiratePointGrid = FIntPoint(-1, -1);
    // Array Position
    TArray<FIntPoint> ArrayPointPosition;
};
