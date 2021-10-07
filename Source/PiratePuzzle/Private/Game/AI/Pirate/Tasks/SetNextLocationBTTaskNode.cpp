// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Pirate/Tasks/SetNextLocationBTTaskNode.h"
#include "AIController.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSetNextLocation, All, All);

USetNextLocationBTTaskNode::USetNextLocationBTTaskNode()
{
    NodeName = "Set Next Location";
}

EBTNodeResult::Type USetNextLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (BlackBoard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        if (!Controller) return EBTNodeResult::Failed;
        const auto TempAICharacter = Cast<APirateAICharacter>(Controller->GetPawn());
        if (!TempAICharacter) return EBTNodeResult::Failed;
        if (TempAICharacter->GetStateAI() == EStateAI::Walk)
        {
            BlackBoard->SetValueAsVector(this->NewLocation.SelectedKeyName, TempAICharacter->GetNextLocation());
            UE_LOG(LogSetNextLocation, Display, TEXT("Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
        }
        else
            return EBTNodeResult::Failed;
    }
    return EBTNodeResult::Succeeded;
}
