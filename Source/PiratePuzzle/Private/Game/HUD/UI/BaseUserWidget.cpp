// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/BaseUserWidget.h"
#include "PPGameInstance.h"
#include "Game/GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

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

void UBaseUserWidget::OpenLevel(FName NameLevel)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerDelayCallBack))
    {
        UE_LOG(LogBaseUserWidget, Display, TEXT("Name: %s | Struct TimerDelayCallBack exist!"), *GetName())
        GetWorld()->GetTimerManager().ClearTimer(this->TimerDelayCallBack);
    }
    UGameplayStatics::OpenLevel(GetWorld(), NameLevel);
}

void UBaseUserWidget::OpenLevelTimer(FName NameLevel, float RateTime)
{
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UBaseUserWidget::OpenLevel, NameLevel);
    GetWorld()->GetTimerManager().SetTimer(this->TimerDelayCallBack, TimerDelegate, RateTime, false);
    UE_LOG(LogBaseUserWidget, Display, TEXT("Name: %s | Next Level: %s | Rate timer: %f "), *GetName(), *NameLevel.ToString(), RateTime);
}

void UBaseUserWidget::ButtonActiveTimer(bool State, float RateTime)
{
    FName NameFunc = (State == true) ? "EnableButtonActive" : "DisableButtonActive";

    FTimerDelegate TempDelegate;
    TempDelegate.BindUFunction(this, NameFunc);
    FTimerHandle TempTimer;
    GetWorld()->GetTimerManager().SetTimer(TempTimer, TempDelegate, RateTime, false);
}

void UBaseUserWidget::PlaySoundCue(USoundCue* PlaySound)
{
    UGameplayStatics::PlaySound2D(GetWorld(), PlaySound);
    UE_LOG(LogBaseUserWidget, Display, TEXT("Name widget: %s | Play sound: %s"), *GetName(), *PlaySound->GetName());
}

void UBaseUserWidget::PlaySoundCueTimer(USoundCue* PlaySound, const float RateTime)
{
    if (RateTime <= 0.0f)
    {
        this->PlaySoundCue(PlaySound);
        UE_LOG(LogBaseUserWidget, Warning, TEXT("Name widget: %s | Rate time <= 0.0f"), *GetName());
        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UBaseUserWidget::PlaySoundCue, PlaySound);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
    UE_LOG(LogBaseUserWidget, Display, TEXT("Name widget: %s | PlaySound on timer: %s | Rate timer: %f"), *GetName(), *PlaySound->GetName(),
        RateTime);
}

void UBaseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->GamePlayMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GamePlayMode, TEXT("Class AGamePlayMode is nullptr"));

    this->PPGameInstance = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
    checkf(this->PPGameInstance, TEXT("Class UPPGameInstance is nullptr"));
}
