// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ResetStateRunBTTaskNode.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UResetStateRunBTTaskNode : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UResetStateRunBTTaskNode();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
