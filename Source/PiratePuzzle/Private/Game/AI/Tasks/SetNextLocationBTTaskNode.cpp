// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Tasks/SetNextLocationBTTaskNode.h"
#include "AIController.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogSetNextLocation, All, All);

USetNextLocationBTTaskNode::USetNextLocationBTTaskNode()
{
    NodeName = "Set Next Location";
}

EBTNodeResult::Type USetNextLocationBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();
    if (!BlackBoard)
    {
        // UE_LOG(LogSetNextLocation, Error, TEXT("Blackboard is nullptr"));
        return EBTNodeResult::Failed;
    }

    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        // UE_LOG(LogSetNextLocation, Error, TEXT("Controller is nullptr"));
        return EBTNodeResult::Failed;
    }

    if (this->SetCharacter == APirateAICharacter::StaticClass())
    {
        APirateAICharacter* TempAICharacter = Cast<APirateAICharacter>(Controller->GetCharacter());
        if (TempAICharacter->GetStateAI() == EStateAI::Walk)
        {
            BlackBoard->SetValueAsVector(this->NewLocation.SelectedKeyName, TempAICharacter->GetNextLocation());
            // UE_LOG(LogSetNextLocation, Display, TEXT("AI pirate Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
            return (EBTNodeResult::Succeeded);
        }
        // UE_LOG(LogSetNextLocation, Error, TEXT("AI pirate current state: %s"), *UEnum::GetValueAsString(TempAICharacter->GetStateAI()));
    }
    else if (this->SetCharacter == ASkeletonRunnerCharacter::StaticClass())
    {
        ASkeletonRunnerCharacter* TempAICharacter = Cast<ASkeletonRunnerCharacter>(Controller->GetCharacter());
        if (TempAICharacter->GetStateAISkeletonRunner() == EStateAI::Walk)
        {
            BlackBoard->SetValueAsVector(this->NewLocation.SelectedKeyName, TempAICharacter->GetNextLocation());
            UE_LOG(LogSetNextLocation, Display, TEXT("AI Skeleton runner Status: %s"), *UEnum::GetValueAsString(EBTNodeResult::Succeeded));
            return (EBTNodeResult::Succeeded);
        }
        // UE_LOG(LogSetNextLocation, Error, TEXT("AI Skeleton runner current state: %s"),
        // *UEnum::GetValueAsString(TempAICharacter->GetStateAISkeletonRunner()));
    }
    // UE_LOG(LogSetNextLocation, Error, TEXT("AI pointer on class character doesn't exist"));
    return EBTNodeResult::Failed;
}
