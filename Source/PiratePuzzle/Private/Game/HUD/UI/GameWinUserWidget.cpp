// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameWinUserWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameWinUserWidget, All, All);

void UGameWinUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->RestartButton->OnClicked.AddDynamic(this, &UGameWinUserWidget::OnClickedRestart);

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
