// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/StartInfoUserWidget.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"

void UStartInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->CloseButton->OnClicked.AddDynamic(this, &UStartInfoUserWidget::CloseWidget);
}

void UStartInfoUserWidget::CloseWidget()
{
    if (!bIsButtonActive) return;
    ShowAnim(this->CloseButtonAnim);
    ChangeGameStateTimer(EGameState::GameProgress, this->DelayCloseButton);
    this->bIsButtonActive = false;
}
