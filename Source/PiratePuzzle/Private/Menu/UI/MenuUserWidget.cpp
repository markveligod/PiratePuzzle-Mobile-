// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/UI/MenuUserWidget.h"
#include "Menu/GameMenuMode.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuUserWidget, All, All);

void UMenuUserWidget::ChangeActiveWidgetTimer(bool NewState, float RateTime)
{
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMenuUserWidget::ChangeActiveWidget, NewState);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
    UE_LOG(LogMenuUserWidget, Display, TEXT("Change Active Widget on timer | Name widget: %s | State: %s | Time: %f"), *GetName(),
        *BaseUtils::GetStringFromBool(NewState), RateTime);
}

void UMenuUserWidget::PlayAnimationTimer(UWidgetAnimation* Anim, float RateTime)
{
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMenuUserWidget::ShowAnim, Anim);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
}

void UMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->GameMode = Cast<AGameMenuMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));

    UE_LOG(LogMenuUserWidget, Display, TEXT("Native On Initialized"));
}
