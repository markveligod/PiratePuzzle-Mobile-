// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameWinUserWidget.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameWinUserWidget, All, All);

void UGameWinUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->RestartButton->OnClicked.AddDynamic(this, &UGameWinUserWidget::OnClickedRestart);
    GetGamePlayMode()->OnCoinIncrease.AddUObject(this, &UGameWinUserWidget::ChangeCountCoin);

    UE_LOG(LogGameWinUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameWinUserWidget::OnClickedRestart()
{
    PlayAnimation(this->RestartButtonAnim);

    FTimerDelegate TimerDelegate;
    const UObject* TempObj = Cast<UObject>(GetWorld());
    FName NameLevel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
    TimerDelegate.BindStatic(&UGameplayStatics::OpenLevel, TempObj, NameLevel, true, FString(TEXT("")));
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayCloseButton, false);
}

void UGameWinUserWidget::ChangeCountCoin(int32 CountCoin)
{
    FSlateBrush NewBrash;
    NewBrash.SetResourceObject(this->GoldTexture);
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
