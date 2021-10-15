// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/BaseUserWidget.h"
#include "Game/GamePlayMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseUserWidget, All, All);

void UBaseUserWidget::ShowAnim(UWidgetAnimation* Anim)
{
    if (!Anim)
    {
        UE_LOG(LogBaseUserWidget, Error, TEXT("Name: %s | Anim pointer is nullptr"), *GetName());
        return;
    }
    PlayAnimation(Anim);
    UE_LOG(LogBaseUserWidget, Display, TEXT("Name: %s | Play animation: %s"), *GetName(), *Anim->GetName());
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerDelayCallBack))
    {
        UE_LOG(LogBaseUserWidget, Display, TEXT("Name: %s | Struct TimerDelayCallBack exist!"), *GetName())
        GetWorld()->GetTimerManager().ClearTimer(this->TimerDelayCallBack);
    }
}

void UBaseUserWidget::ShowAnimTimer(UWidgetAnimation* Anim, float RateTime)
{
    if (!Anim)
    {
        UE_LOG(LogBaseUserWidget, Error, TEXT("Name: %s | Anim pointer is nullptr"), *GetName());
        return;
    }
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, FName("ShowAnim"), Anim);
    GetWorld()->GetTimerManager().SetTimer(this->TimerDelayCallBack, TimerDelegate, RateTime, false);
    UE_LOG(LogBaseUserWidget, Display, TEXT("Name: %s | Play animation: %s | Time: %f"), *GetName(), *Anim->GetName(), RateTime);
}

void UBaseUserWidget::ButtonActiveTimer(bool State, float RateTime)
{
    FName NameFunc = (State == true) ? "EnableButtonActive" : "DisableButtonActive";

    FTimerDelegate TempDelegate;
    TempDelegate.BindUFunction(this, NameFunc);
    FTimerHandle TempTimer;
    GetWorld()->GetTimerManager().SetTimer(TempTimer, TempDelegate, RateTime, false);
}

void UBaseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->GamePlayMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GamePlayMode, TEXT("class AGamePlayMode is nullptr"));
}
