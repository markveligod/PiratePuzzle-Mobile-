// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Pirate/Tasks/ResetStateRunBTTaskNode.h"
#include "AIController.h"
#include "Game/AI/Pirate/PirateAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogResetRunTaskNode, All, All);

UResetStateRunBTTaskNode::UResetStateRunBTTaskNode()
{
    NodeName = "Reset State Run";
}

EBTNodeResult::Type UResetStateRunBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto TempAIController = OwnerComp.GetAIOwner();
    if (!TempAIController) return EBTNodeResult::Failed;

    const auto TempAICharacter = Cast<APirateAICharacter>(TempAIController->GetPawn());

    if (!TempAICharacter) return EBTNodeResult::Failed;

    TempAICharacter->SetStateAI(EStateAI::Idle);

    UE_LOG(LogResetRunTaskNode, Display, TEXT("Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
    return (EBTNodeResult::Succeeded);
}
