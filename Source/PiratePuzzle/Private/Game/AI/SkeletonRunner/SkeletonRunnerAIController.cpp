// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/SkeletonRunner/SkeletonRunnerAIController.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"

void ASkeletonRunnerAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    const auto AICharacter = Cast<ASkeletonRunnerCharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
