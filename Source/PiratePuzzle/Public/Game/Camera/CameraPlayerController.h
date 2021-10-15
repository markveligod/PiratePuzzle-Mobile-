// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/GameDataTypes.h"
#include "GameFramework/PlayerController.h"
#include "CameraPlayerController.generated.h"

class AGamePlayMode;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API ACameraPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    // Pointer on current Game mode
    AGamePlayMode* GameMode;

    // Function for accepting a new state
    void OnChangeGameState(EGameState NewState);
};
