// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameOverUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameOverUserWidget, All, All);

void UGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->RestartButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::OnClickedRestart);

    UE_LOG(LogGameOverUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameOverUserWidget::OnClickedRestart()
{
    PlayAnimation(this->RestartButtonAnim);

    FTimerDelegate TimerDelegate;
    const UObject* TempObj = Cast<UObject>(GetWorld());
    FName NameLevel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
    TimerDelegate.BindStatic(&UGameplayStatics::OpenLevel, TempObj, NameLevel, true, FString(TEXT("")));
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayCloseButton, false);
}
