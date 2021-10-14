// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameUserWidget.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameUserWidget, All, All);

void UGameUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->CameraButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedSwapCamera);
    this->BackButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedBackPos);
    this->PauseButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedPauseState);

    UE_LOG(LogGameUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameUserWidget::OnClickedSwapCamera()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    ButtonActiveTimer(true, DelayCloseButton);
    PlayAnimation(this->CameraButtonAnim);

    GetGamePlayMode()->GetCameraPawn()->StartSwapCamera();
}

void UGameUserWidget::OnClickedBackPos()
{
    if (!GetStateActiveButton()) return;
    FIntPoint LastPoint = GetGamePlayMode()->GetCameraPawn()->GetAIPirate()->GetLastPositionPoint();
    if (LastPoint == FIntPoint(-1, -1)) return;
    DisableButtonActive();
    ButtonActiveTimer(true, DelayCloseButton);
    PlayAnimation(this->BackButtonAnim);

    GetGamePlayMode()->GetCameraPawn()->StartMoveAICharacterOnPos(LastPoint);
}

void UGameUserWidget::OnClickedPauseState()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    PlayAnimation(this->PauseButtonAnim);
    ChangeGameStateTimer(EGameState::Pause, DelayCloseButton);
}
