// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetNextLocationBTTaskNode.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API USetNextLocationBTTaskNode : public UBTTaskNode
{
    GENERATED_BODY()
public:
    USetNextLocationBTTaskNode();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector NewLocation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<ACharacter> SetCharacter;
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
