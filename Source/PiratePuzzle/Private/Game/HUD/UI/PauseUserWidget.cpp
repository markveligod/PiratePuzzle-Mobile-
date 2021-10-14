// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/PauseUserWidget.h"
#include "Components/Button.h"

DEFINE_LOG_CATEGORY_STATIC(LogPauseUserWidget, All, All);

void UPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->CloseButton->OnClicked.AddDynamic(this, &UPauseUserWidget::OnClickedCloseWidget);

    UE_LOG(LogPauseUserWidget, Display, TEXT("Native On Initialized"));
}

void UPauseUserWidget::OnClickedCloseWidget()
{
    if (!this->GetStateActiveButton()) return;
    this->DisableButtonActive();

    PlayAnimation(this->CloseButtonAnim);
    ChangeGameStateTimer(EGameState::GameProgress, this->DelayCloseButton);
}
