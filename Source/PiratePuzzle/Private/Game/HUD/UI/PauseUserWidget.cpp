// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/PauseUserWidget.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogPauseUserWidget, All, All);

void UPauseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->CloseButton->OnClicked.AddDynamic(this, &UPauseUserWidget::OnClickedCloseWidget);
    this->HomeButton->OnClicked.AddDynamic(this, &UPauseUserWidget::OnClickedHome);
    this->RestartButton->OnClicked.AddDynamic(this, &UPauseUserWidget::OnClickedRestart);

    // Setup music and sound checkbox
    this->MusicCheckBox->SetCheckedState(
        (GetPPGameInstance()->GetMusicClass()->Properties.Volume == 0.0f) ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
    this->SoundCheckBox->SetCheckedState(
        (GetPPGameInstance()->GetSoundClass()->Properties.Volume == 0.0f) ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
    this->MusicCheckBox->OnCheckStateChanged.AddDynamic(this, &UPauseUserWidget::UpdateMusicVolumeFromCheckBox);
    this->SoundCheckBox->OnCheckStateChanged.AddDynamic(this, &UPauseUserWidget::UpdateSoundVolumeFromCheckBox);

    UE_LOG(LogPauseUserWidget, Display, TEXT("Native On Initialized"));
}

void UPauseUserWidget::OnClickedRestart()
{
    PlayAnimation(this->RestartButtonAnim);

    const FName NameLevel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
    OpenLevelTimer(NameLevel, this->RestartButtonAnim->GetEndTime());
}

void UPauseUserWidget::OnClickedCloseWidget()
{
    if (!this->GetStateActiveButton()) return;
    this->DisableButtonActive();

    PlayAnimation(this->CloseButtonAnim);
    GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameProgress, this->DelayCloseButton);
}

void UPauseUserWidget::OnClickedHome()
{
    ShowAnim(this->HomeButtonAnim);
    OpenLevelTimer(GetPPGameInstance()->GetNameMenuLevel(), this->HomeButtonAnim->GetEndTime());
}

void UPauseUserWidget::UpdateMusicVolumeFromCheckBox(bool bIsChecked)
{
    GetPPGameInstance()->GetMusicClass()->Properties.Volume = (bIsChecked) ? GetPPGameInstance()->GetDefaultMusicVolume() : 0.0f;
}

void UPauseUserWidget::UpdateSoundVolumeFromCheckBox(bool bIsChecked)
{
    GetPPGameInstance()->GetSoundClass()->Properties.Volume = (bIsChecked) ? GetPPGameInstance()->GetDefaultSoundVolume() : 0.0f;
}
