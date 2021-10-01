// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/AI/Pirate/PirateDataTypes.h"
#include "PirateAICharacter.generated.h"

class UBehaviorTree;
UCLASS()
class PIRATEPUZZLE_API APirateAICharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APirateAICharacter();

    // Pointer on Behavior Tree
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree")
    UBehaviorTree* BehaviorTreeAsset;

    // Get and Set change position ai character on grid
    const FIntPoint& GetPosPlayer() const { return (this->PosPlayer); }
    void SetNewPosPlayer(const FIntPoint& NewPoint) { this->PosPlayer = NewPoint; }

    // Get and Set the Location
    const FVector& GetNextLocation() const { return (this->NextLocation); }
    // Setting Next location FVector for walk
    void SetNextLocation(FVector NewLoc) { this->NextLocation = NewLoc; }

    // Get and Set AI State
    UFUNCTION(BlueprintCallable)
    const EStateAI& GetStateAI() const { return (this->StateAI); }

    void SetStateAI(EStateAI NewState) { this->StateAI = NewState; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Position Player on grid
    FIntPoint PosPlayer;

    // enum State Ai character
    EStateAI StateAI = EStateAI::None;

    // Value new location for function move to
    FVector NextLocation;
};
