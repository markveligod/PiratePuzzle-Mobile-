// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameUserWidget.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "UtilsLib/BaseUtils.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameUserWidget, All, All);

void UGameUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->CameraButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedSwapCamera);
    this->BackButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedBackPos);
    this->PauseButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedPauseState);

    GetGamePlayMode()->OnCoinIncrease.BindUObject(this, &UGameUserWidget::ChangeCountCoin);
    
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
    DisableButtonActive();
    ButtonActiveTimer(true, DelayCloseButton);

    APirateAICharacter* TempPirate = GetGamePlayMode()->GetCameraPawn()->GetAIPirate();
    if (TempPirate->GetStateAI() != EStateAI::Idle) return;

    FIntPoint LastPoint = GetGamePlayMode()->GetCameraPawn()->GetAIPirate()->GetLastPositionPoint();
    if (LastPoint == FIntPoint(-1, -1)) return;
    
    PlayAnimation(this->BackButtonAnim);
    GetGamePlayMode()->GetCameraPawn()->StartMoveAICharacterOnPos(LastPoint);
}

void UGameUserWidget::OnClickedPauseState()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    PlayAnimation(this->PauseButtonAnim);
    GetGamePlayMode()->OnChangeGameStateTimer(EGameState::Pause, DelayCloseButton);
}

void UGameUserWidget::ChangeCountCoin(int32 CountCoin)
{
    FSlateBrush NewBrash;
    NewBrash.SetResourceObject(this->GoldTexture);
    NewBrash.SetImageSize(this->SizeGoldTexture);
    if (CountCoin == 1)
    {
        this->ImageOneCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageOneCoinAnim);
    }
    else if (CountCoin == 2)
    {
        this->ImageTwoCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageTwoCoinAnim);
    }
    else if (CountCoin == 3)
    {
        this->ImageThreeCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageThreeCoinAnim);
    }
}
