// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/GameMenuMode.h"
#include "Menu/MenuHUD.h"
#include "Menu/MenuPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameMenuMode, All, All);

AGameMenuMode::AGameMenuMode()
{
    HUDClass = AMenuHUD::StaticClass();
    PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void AGameMenuMode::StartPlay()
{
    Super::StartPlay();

    this->ChangeMenuState(EMenuState::Welcome);
}

void AGameMenuMode::ChangeMenuState(EMenuState NewState)
{
    if (this->MenuState == NewState)
    {
        UE_LOG(
            LogGameMenuMode, Warning, TEXT("The current state of the level is equal to NewState (%s)"), *UEnum::GetValueAsString(NewState));
        return;
    }

    this->MenuState = NewState;
    this->OnMenuStateChanged.Broadcast(NewState);
}

void AGameMenuMode::ChangeMenuStateTimer(EMenuState NewState, float RateTime)
{
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &AGameMenuMode::ChangeMenuState, NewState);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
}
