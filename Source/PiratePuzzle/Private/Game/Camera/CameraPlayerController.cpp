// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Camera/CameraPlayerController.h"
#include "Game/GamePlayMode.h"

void ACameraPlayerController::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameMode->OnGameStateChanged.AddUObject(this, &ACameraPlayerController::OnChangeGameState);
    bShowMouseCursor = true;
}

void ACameraPlayerController::OnChangeGameState(EGameState NewState)
{
    if (NewState == EGameState::GameProgress)
    {
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
