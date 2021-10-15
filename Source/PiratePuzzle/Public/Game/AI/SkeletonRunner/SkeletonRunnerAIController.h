// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SkeletonRunnerAIController.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API ASkeletonRunnerAIController : public AAIController
{
    GENERATED_BODY()

protected:
    // Called when gaining control of a ai character
    virtual void OnPossess(APawn* InPawn) override;
};
