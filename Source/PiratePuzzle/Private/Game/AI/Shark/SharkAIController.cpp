// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Shark/SharkAIController.h"
#include "Game/AI/Shark/SharkCharacter.h"

void ASharkAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    const auto AICharacter = Cast<ASharkCharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
