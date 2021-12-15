// Is the property of WerFEST Software

#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogPiratePawn, All, All);

FIntPoint APiratePawn::GetPreviousPoint()
{
    if (this->ArrayPointPosition.Num() > 0)
    {
        FIntPoint TempPoint(this->ArrayPointPosition.Last());
        this->ArrayPointPosition.RemoveAt(this->ArrayPointPosition.Num() - 1);
        UE_LOG(LogPiratePawn, Display, TEXT("Name pawn: %s | Return last point: %s"), *GetName(), *TempPoint.ToString());
        return (TempPoint);
    }
    return (FIntPoint(-1, -1));
}

void APiratePawn::ChangeStateBrain(EStateBrain NewState)
{
    Super::ChangeStateBrain(NewState);
    this->OnStateBrainChanged.Broadcast(NewState);
}

void APiratePawn::BeginPlay()
{
    Super::BeginPlay();
}
