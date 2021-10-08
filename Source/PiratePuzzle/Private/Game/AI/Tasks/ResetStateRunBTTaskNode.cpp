// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Tasks/ResetStateRunBTTaskNode.h"
#include "AIController.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogResetRunTaskNode, All, All);

UResetStateRunBTTaskNode::UResetStateRunBTTaskNode()
{
    NodeName = "Reset State Run";
}

EBTNodeResult::Type UResetStateRunBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (this->ResetCharacter == (APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* Character = Cast<APirateAICharacter>(OwnerComp.GetAIOwner()->GetCharacter());
        Character->SetStateAI(EStateAI::Idle);
        UE_LOG(LogResetRunTaskNode, Display, TEXT("AI pirate Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
        return (EBTNodeResult::Succeeded);
    }
    else if (this->ResetCharacter == (ASkeletonRunnerCharacter::StaticClass()))
    {
        ASkeletonRunnerCharacter* Character = Cast<ASkeletonRunnerCharacter>(OwnerComp.GetAIOwner()->GetCharacter());
        Character->SetNewStateAISkeletonRunner(EStateAI::Idle);
        UE_LOG(LogResetRunTaskNode, Display, TEXT("Skeleton runner Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
        return (EBTNodeResult::Succeeded);
    }
    // UE_LOG(LogResetRunTaskNode, Error, TEXT("Type reset character doesn't exist("));
    return (EBTNodeResult::Failed);
}
