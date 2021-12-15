// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameWinUserWidget.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "UtilsLib/LoaderUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameWinUserWidget, All, All);

void UGameWinUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->RestartButton->OnClicked.AddDynamic(this, &UGameWinUserWidget::OnClickedRestart);
    GetGamePlayMode()->OnCoinIncrease.AddUObject(this, &UGameWinUserWidget::ChangeCountCoin);
    this->HomeButton->OnClicked.AddDynamic(this, &UGameWinUserWidget::OnClickedHome);
    this->NextLevelButton->OnClicked.AddDynamic(this, &UGameWinUserWidget::OnClickedNextLevel);

    const int32 NextIndexLevel = GetPPGameInstance()->GetRunLevel() + 1;
    if (GetPPGameInstance()->GetInfoLevel(NextIndexLevel).NameLevel == NAME_None)
    {
        this->NextLevelButton->SetVisibility(ESlateVisibility::Hidden);
    }

    GetGamePlayMode()->OnGameStateChanged.AddUObject(this, &UGameWinUserWidget::OnChangeGameState);
    this->OpenCoinImage->SetVisibility(ESlateVisibility::Hidden);
    this->NextLevelButton->SetIsEnabled(true);

    UE_LOG(LogGameWinUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameWinUserWidget::OnClickedRestart()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();

    PlayAnimation(this->RestartButtonAnim);

    const FName NameLevel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
    OpenLevelTimer(NameLevel, this->RestartButtonAnim->GetEndTime());
}

void UGameWinUserWidget::ChangeCountCoin(const int32 CountCoin)
{
    FSlateBrush NewBrash;
    NewBrash.SetResourceObject(LoaderUtils::LoaderSyncObject(GetWorld(), this->GoldTexture));
    NewBrash.SetImageSize(this->SizeGoldTexture);
    if (CountCoin == 1)
    {
        this->CoinOneImage->SetBrush(NewBrash);
    }
    else if (CountCoin == 2)
    {
        this->CoinTwoImage->SetBrush(NewBrash);
    }
    else if (CountCoin == 3)
    {
        this->CoinThreeImage->SetBrush(NewBrash);
    }
}

void UGameWinUserWidget::OnClickedHome()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();

    ShowAnim(this->HomeButtonAnim);
    OpenLevelTimer(GetPPGameInstance()->GetNameMenuLevel(), this->HomeButtonAnim->GetEndTime());
}

void UGameWinUserWidget::OnClickedNextLevel()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();

    const int32 NextIndexLevel = GetPPGameInstance()->GetRunLevel() + 1;
    const FName NameLevel = GetPPGameInstance()->GetInfoLevel(NextIndexLevel).NameLevel;
    GetPPGameInstance()->SetRunLevel(NextIndexLevel);

    ShowAnim(this->NextLevelButtonAnim);
    OpenLevelTimer(NameLevel, this->NextLevelButtonAnim->GetEndTime());
}

void UGameWinUserWidget::OnChangeGameState(EGameState NewGameState)
{
    if (NewGameState == EGameState::GameWin)
    {
        if (GetGamePlayMode()->GetCountCoin() == 0)
        {
            this->OpenCoinImage->SetVisibility(ESlateVisibility::Visible);
            this->PlayAnimation(this->ImageCoinAnim, 0.0f, 0);
            this->NextLevelButton->SetIsEnabled(false);
        }
    }
}
