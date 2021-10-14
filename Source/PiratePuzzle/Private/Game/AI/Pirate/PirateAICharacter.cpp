// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Pirate/PirateAICharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogPirateAICharacter, All, All);

// Sets default values
APirateAICharacter::APirateAICharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

FIntPoint APirateAICharacter::GetLastPositionPoint()
{
    if (this->ArrayPointPos.Num() > 0 && this->StateAI == EStateAI::Idle)
    {
        FIntPoint TempPoint(this->ArrayPointPos.Last());
        this->ArrayPointPos.RemoveAt(this->ArrayPointPos.Num() - 1);
        UE_LOG(LogPirateAICharacter, Display, TEXT("Return last point: %s"), *TempPoint.ToString());
        for (int32 i = 0; i < this->ArrayPointPos.Num(); i++)
        {
            UE_LOG(LogPirateAICharacter, Display, TEXT("Index #%i | Pos point in array: %s"), i, *this->ArrayPointPos[i].ToString());
        }
        return (TempPoint);
    }
    return (FIntPoint(-1, -1));
}

// Called when the game starts or when spawned
void APirateAICharacter::BeginPlay()
{
    Super::BeginPlay();
    this->StateAI = EStateAI::Idle;
}
