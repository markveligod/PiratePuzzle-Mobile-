// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Pirate/PirateAIController.h"
#include "Game/AI/Pirate/PirateAICharacter.h"

void APirateAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    const auto AICharacter = Cast<APirateAICharacter>(InPawn);
    if (AICharacter)
    {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset);
    }
}
