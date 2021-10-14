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

    UE_LOG(LogGameUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameUserWidget::OnClickedSwapCamera()
{
    GetGamePlayMode()->GetCameraPawn()->StartSwapCamera();
}

void UGameUserWidget::OnClickedBackPos()
{
    FIntPoint LastPoint = GetGamePlayMode()->GetCameraPawn()->GetAIPirate()->GetLastPositionPoint();
    if (LastPoint == FIntPoint(-1, -1)) return;

    GetGamePlayMode()->GetCameraPawn()->StartMoveAICharacterOnPos(LastPoint);
}
