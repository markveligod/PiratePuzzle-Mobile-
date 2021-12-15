// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/UI/MenuUserWidget.h"
#include "PPGameInstance.h"
#include "Menu/GameMenuMode.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuUserWidget, All, All);

void UMenuUserWidget::ChangeActiveWidgetTimer(bool NewState, float RateTime)
{
    if (RateTime == 0.0f)
    {
        UE_LOG(LogMenuUserWidget, Warning, TEXT("Call ChangeActiveWidgetTimer with 0.0f | RateTime == %f"), RateTime);
        this->ChangeActiveWidget(NewState);
        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMenuUserWidget::ChangeActiveWidget, NewState);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
    UE_LOG(LogMenuUserWidget, Display, TEXT("Change Active Widget on timer | Name widget: %s | State: %s | Time: %f"), *GetName(),
        *BaseUtils::GetStringFromBool(NewState), RateTime);
}

void UMenuUserWidget::PlayAnimationTimer(UWidgetAnimation* Anim, float RateTime)
{
    if (RateTime == 0.0f)
    {
        UE_LOG(LogMenuUserWidget, Warning, TEXT("Call PlayAnimationTimer with 0.0f | RateTime == %f"), RateTime);
        this->ShowAnim(Anim);
        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMenuUserWidget::ShowAnim, Anim);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
}

void UMenuUserWidget::StartLevelTimer(FName NameLevel, float TimeRate)
{
    if (this->GameInstance->IsFirstRunGame()) this->GameInstance->SetStateFirstRunGame(false);

    this->PlayAnimationTimer(this->EndAnim, TimeRate);

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UMenuUserWidget::StartLevel, NameLevel);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, this->EndAnim->GetEndTime() + TimeRate, false);
    UE_LOG(LogMenuUserWidget, Display, TEXT("Start Level on timer | Name widget: %s | Name Level: %s | Time: %f"), *GetName(),
        *NameLevel.ToString(), this->EndAnim->GetEndTime() + TimeRate);
}

void UMenuUserWidget::PlaySoundTimer(USoundBase* SoundToPlay, float RateTime)
{
    if (RateTime == 0.0f)
    {
        UE_LOG(LogMenuUserWidget, Warning, TEXT("Call PlaySoundTimer with 0.0f | RateTime == %f"), RateTime);
        PlaySound(SoundToPlay);
        return;
    }
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UUserWidget::PlaySound, SoundToPlay);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
    UE_LOG(LogMenuUserWidget, Display, TEXT("Play Sound on timer | Name widget: %s | Name sound: %s | Time: %f"), *GetName(),
        *SoundToPlay->GetName(), RateTime);
}

void UMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->GameMode = Cast<AGameMenuMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameInstance = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
    checkf(this->GameInstance, TEXT("Game Instance is nullptr"));

    UE_LOG(LogMenuUserWidget, Display, TEXT("Native On Initialized"));
}

void UMenuUserWidget::StartLevel(FName NameLevel) const
{

    UGameplayStatics::OpenLevel(GetWorld(), NameLevel);
}
