// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/LoadingUserWidget.h"

#include "Components/Image.h"
#include "GameFramework/GameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLoadingUserWidget, All, All);

void ULoadingUserWidget::EnableButtonActive()
{
    Super::EnableButtonActive();
    GetWorld()->GetTimerManager().SetTimer(this->TimerChangeImage, this, &ULoadingUserWidget::ChangeLoadingImage, this->TimeRate, true);
    UE_LOG(LogLoadingUserWidget, Display, TEXT("Enable button Active"));
}

void ULoadingUserWidget::DisableButtonActive()
{
    Super::DisableButtonActive();
    GetWorld()->GetTimerManager().ClearTimer(this->TimerChangeImage);
    UE_LOG(LogLoadingUserWidget, Display, TEXT("Disable button Active"));
}

void ULoadingUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    const auto UserSettings = UGameUserSettings::GetGameUserSettings();
    this->ImageSize = FVector2D(UserSettings->GetScreenResolution());
    UE_LOG(LogLoadingUserWidget, Display, TEXT("Native On Initialized"));
}

void ULoadingUserWidget::ChangeLoadingImage()
{
    FSlateBrush Brush;
    Brush.SetResourceObject(this->ArrayPictureSamples[this->CountPicture]);
    Brush.SetImageSize(this->ImageSize);
    this->LoadingImage->SetBrush(Brush);
    this->CountPicture = ((this->CountPicture + 1) == ArrayPictureSamples.Num()) ? 0 : this->CountPicture + 1;
}
