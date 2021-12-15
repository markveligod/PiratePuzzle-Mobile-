// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/UI/GeneralMenuUserWidget.h"
#include "Components/Button.h"
#include "Menu/UI/Components/MenuButton.h"
#include "PPGameInstance.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UtilsLib/BaseUtils.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CheckBox.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeneralMenuUserWidget, All, All);

void UGeneralMenuUserWidget::ChangeActiveWidget(bool bNewState)
{
    Super::ChangeActiveWidget(bNewState);
    if (bNewState)
    {
        PlayAnimation(this->LoopWheelImage, 0.f, 0);
        if (!GetPPGameInstance()->IsFirstRunGame() && !this->bAnimPlayButton)
        {
            this->OnClickedVisibleLevels();
        }
    }
}

void UGeneralMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("---|Native On Initialized Start|---"));

    // Play overlay init
    this->PlayButton->OnClicked.AddDynamic(this, &UGeneralMenuUserWidget::OnClickedVisibleLevels);

    // Settings overlay init
    this->SettingsButton->OnClicked.AddDynamic(this, &UGeneralMenuUserWidget::VisibleSettings);
    this->CloseSettingsButton->OnClicked.AddDynamic(this, &UGeneralMenuUserWidget::HiddenSettings);

    // Next and Prev button init
    this->NextButton->OnClicked.AddDynamic(this, &UGeneralMenuUserWidget::SetNextListButtons);
    this->PrevButton->OnClicked.AddDynamic(this, &UGeneralMenuUserWidget::SetPrevListButtons);
    this->NextButton->SetRenderOpacity(0.0f);
    this->PrevButton->SetRenderOpacity(0.0f);

    // Setup music and sound checkbox
    this->MusicCheckBox->SetCheckedState(
        (GetPPGameInstance()->GetMusicClass()->Properties.Volume == 0.0f) ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
    this->SoundCheckBox->SetCheckedState(
        (GetPPGameInstance()->GetSoundClass()->Properties.Volume == 0.0f) ? ECheckBoxState::Unchecked : ECheckBoxState::Checked);
    this->MusicCheckBox->OnCheckStateChanged.AddDynamic(this, &UGeneralMenuUserWidget::UpdateMusicVolumeFromCheckBox);
    this->SoundCheckBox->OnCheckStateChanged.AddDynamic(this, &UGeneralMenuUserWidget::UpdateSoundVolumeFromCheckBox);
    PlaySoundTimer(this->MusicBg, this->PlayBgRateTime);

    // Add button and TextBlock in Map;
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->FirstPosition, this->FirstPositionTextBlock, this->AnimFirstButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->SecondPosition, this->SecondPositionTextBlock, this->AnimSecondButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->ThirdPosition, this->ThirdPositionTextBlock, this->AnimThirdButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->FourthPosition, this->FourthPositionTextBlock, this->AnimFourthButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->FifthPosition, this->FifthPositionTextBlock, this->AnimFifthButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->SixthPosition, this->SixthPositionTextBlock, this->AnimSixthButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->SeventhPosition, this->SeventhPositionTextBlock, this->AnimSeventhButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->EighthPosition, this->EighthPositionTextBlock, this->AnimEighthButton));
    this->ArrayInfoMenuButtons.Add(FInfoMenuButton(this->NinthPosition, this->NinthPositionTextBlock, this->AnimNinthButton));

    this->SettingsButton->SetVisibility(ESlateVisibility::Hidden);
    this->NextButton->SetVisibility(ESlateVisibility::Hidden);
    this->BackgroundImage->SetVisibility(ESlateVisibility::Hidden);
    this->PrevButton->SetVisibility(ESlateVisibility::Hidden);

    // Starting interval initialization
    this->MapAllLevels = GetPPGameInstance()->GetAllLevels();
    if (this->MapAllLevels.Num() == 0)
    {
        UE_LOG(LogGeneralMenuUserWidget, Error, TEXT("Map levels is doesn't contains any level | Num map levels: %i"),
            this->MapAllLevels.Num());
        return;
    }
    this->RangeLevels.Min = 1;
    this->RangeLevels.Max = (this->MapAllLevels.Num() > this->CountList) ? this->CountList : this->MapAllLevels.Num();
    int32 NumLastPassLevel = 0;
    for (const auto Map : this->MapAllLevels)
    {
        if (Map.Value.LevelState == ELevelState::ZeroGold)
        {
            NumLastPassLevel = Map.Key;
            UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("Num last pass level: %s | Index: %i"), *Map.Value.NameLevel.ToString(),
                NumLastPassLevel);
            break;
        }
    }

    while (true)
    {
        if (NumLastPassLevel >= this->RangeLevels.Min && NumLastPassLevel <= this->RangeLevels.Max) break;
        TInterval<int32> TempInterval;
        TempInterval.Min = this->RangeLevels.Min + this->CountList;
        TempInterval.Max = TempInterval.Min + this->CountList - 1;
        if (!this->MapAllLevels.Contains(TempInterval.Max))
        {
            TempInterval.Max = this->MapAllLevels.Num();
        }
        this->RangeLevels = TempInterval;
    }
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("Value interval | Min: %i | Max: %i"), this->RangeLevels.Min, this->RangeLevels.Max);
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("---|Native On Initialized End|---"));
}

void UGeneralMenuUserWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("---|Native Construct Start|---"));

    // Setup Base bind configuration
    for (const auto InfoMenuButton : this->ArrayInfoMenuButtons)
    {
        InfoMenuButton.Button->SetPlayAnim(InfoMenuButton.AnimPlay);
        InfoMenuButton.Button->OnMenuClick.AddDynamic(this, &UGeneralMenuUserWidget::StartLevel);
        InfoMenuButton.Button->SetCanvasPanelSlot(Cast<UCanvasPanelSlot>(InfoMenuButton.Button->Slot));
        UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("Name button: %s | Has canvas panel slots: %s"), *InfoMenuButton.Button->GetName(),
            *BaseUtils::GetStringFromBool(InfoMenuButton.Button->GetCanvasPanelSlot() != nullptr));
    }
    this->UpdateConfigurationMap();
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("---|Native Construct End|---"));
}

void UGeneralMenuUserWidget::OnClickedVisibleLevels()
{
    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);
    this->bAnimPlayButton = true;

    this->SettingsButton->SetVisibility(ESlateVisibility::Visible);
    this->NextButton->SetVisibility(ESlateVisibility::Visible);
    this->BackgroundImage->SetVisibility(ESlateVisibility::Visible);
    this->PrevButton->SetVisibility(ESlateVisibility::Visible);

    StopAllAnimations();
    ShowAnim(this->AnimPlayButton);
    PlayAnimation(this->LoopWheelImage, 0.f, 0);
    this->UpdateConfigurationMap();
    PlayAnimationTimer(this->AnimUpStartOverlay, this->AnimPlayButton->GetEndTime());
    ChangeActiveWidgetTimer(true, this->AnimUpStartOverlay->GetEndTime() + this->AnimPlayButton->GetEndTime());
}

void UGeneralMenuUserWidget::VisibleSettings()
{
    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);

    ShowAnim(this->AnimSettingsButton);
    PlayAnimationTimer(this->AnimVisibleSettingsOverlay, this->AnimSettingsButton->GetEndTime());

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UGeneralMenuUserWidget::ChangeVisibleButtons, ESlateVisibility::Hidden);
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, TimerDelegate, this->AnimSettingsButton->GetEndTime() + this->AnimVisibleSettingsOverlay->GetEndTime(), false);

    ChangeActiveWidgetTimer(true, this->AnimSettingsButton->GetEndTime() + this->AnimVisibleSettingsOverlay->GetEndTime());
}

void UGeneralMenuUserWidget::HiddenSettings()
{
    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);

    ShowAnim(this->AnimCloseSettingsButton);
    PlayAnimationTimer(this->AnimHiddenSettingsOverlay, this->AnimCloseSettingsButton->GetEndTime());

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UGeneralMenuUserWidget::ChangeVisibleButtons, ESlateVisibility::Visible);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, this->AnimCloseSettingsButton->GetEndTime(), false);

    ChangeActiveWidgetTimer(true, this->AnimCloseSettingsButton->GetEndTime() + this->AnimHiddenSettingsOverlay->GetEndTime());
}

void UGeneralMenuUserWidget::ChangeVisibleButtons(ESlateVisibility NewVis)
{
    int32 CountPos = this->RangeLevels.Min;
    for (auto InfoMenuButton : this->ArrayInfoMenuButtons)
    {
        if (CountPos > this->RangeLevels.Max)  // if the level in the map doesn't exists
            break;
        InfoMenuButton.TextBlock->SetVisibility(NewVis);
        InfoMenuButton.Button->SetVisibility(NewVis);
        CountPos++;
    }
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("All button set visible: %s"), *UEnum::GetValueAsString(NewVis));
}

void UGeneralMenuUserWidget::SetNextListButtons()
{
    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);
    ShowAnim(this->AnimNextButton);
    ChangeActiveWidgetTimer(true, this->AnimNextButton->GetEndTime());

    TInterval<int32> TempInterval;
    TempInterval.Min = this->RangeLevels.Min + this->CountList;
    TempInterval.Max = TempInterval.Min + this->CountList - 1;
    if (!this->MapAllLevels.Contains(TempInterval.Min)) return;
    if (!this->MapAllLevels.Contains(TempInterval.Max))
    {
        TempInterval.Max = this->MapAllLevels.Num();
    }
    this->RangeLevels = TempInterval;
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("New range levels on map: %i - %i"), TempInterval.Min, TempInterval.Max);
    this->UpdateConfigurationMap();
}

void UGeneralMenuUserWidget::SetPrevListButtons()
{
    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);
    ShowAnim(this->AnimPrevButton);
    ChangeActiveWidgetTimer(true, this->AnimPrevButton->GetEndTime());

    TInterval<int32> TempInterval;
    TempInterval.Min = this->RangeLevels.Min - this->CountList;
    TempInterval.Max = TempInterval.Min + this->CountList - 1;
    if (!this->MapAllLevels.Contains(TempInterval.Min)) return;
    if (!this->MapAllLevels.Contains(TempInterval.Max))
    {
        TempInterval.Max = this->MapAllLevels.Num();
    }
    this->RangeLevels = TempInterval;
    UE_LOG(LogGeneralMenuUserWidget, Display, TEXT("New range levels on map: %i - %i"), TempInterval.Min, TempInterval.Max);
    this->UpdateConfigurationMap();
}

void UGeneralMenuUserWidget::StartLevel(int32 IndexLevel, UWidgetAnimation* AnimToPlayButton)
{
    const auto InfoLevel = GetPPGameInstance()->GetInfoLevel(IndexLevel);
    if (InfoLevel.LevelState == ELevelState::Lock) return;

    if (!IsWidgetActive()) return;
    ChangeActiveWidget(false);

    if (!AnimToPlayButton)
    {
        UE_LOG(LogGeneralMenuUserWidget, Error, TEXT("Widget animation is nullptr"));
        ChangeActiveWidget(true);
        return;
    }

    GetPPGameInstance()->SetRunLevel(IndexLevel);
    ShowAnim(AnimToPlayButton);
    StartLevelTimer(InfoLevel.NameLevel, AnimToPlayButton->GetEndTime());
}

void UGeneralMenuUserWidget::UpdateConfigurationMap()
{
    this->MaxTextBlock->SetText(FText::FromString(FString::FromInt(this->RangeLevels.Max)));
    this->MinTextBlock->SetText(FText::FromString(FString::FromInt(this->RangeLevels.Min)));

    int32 CountPos = this->RangeLevels.Min;
    for (auto InfoMenuButton : this->ArrayInfoMenuButtons)
    {

        if (CountPos <= this->RangeLevels.Max)  // if the level in the map exists
        {
            if (CountPos > this->MapAllLevels.Num()) return;  // if interval > num map levels

            const FInfoLevel TempInfoLevel = this->MapAllLevels[CountPos];
            if (TempInfoLevel.LevelState != ELevelState::Lock)
            {
                this->UpdateSettingsButton(InfoMenuButton, FText::FromString(FString::FromInt(CountPos)), ESlateVisibility::Visible,
                    CountPos, this->GetPictureFromLevelState(TempInfoLevel.LevelState), this->ButtonSizeOther);
            }
            else
            {
                this->UpdateSettingsButton(
                    InfoMenuButton, FText::FromString(""), ESlateVisibility::Hidden, CountPos, this->LockTexture, this->ButtonSizeLock);
            }
        }
        else
        {
            InfoMenuButton.TextBlock->SetText(FText::FromString(""));
            InfoMenuButton.TextBlock->SetVisibility(ESlateVisibility::Hidden);
            InfoMenuButton.Button->SetVisibility(ESlateVisibility::Hidden);
        }
        CountPos++;
    }
}

UTexture2D* UGeneralMenuUserWidget::GetPictureFromLevelState(ELevelState LevelState) const
{
    switch (LevelState)
    {
        case ELevelState::ZeroGold: return (this->ZeroGoldTexture);
        case ELevelState::OneGold: return (this->OneGoldTexture);
        case ELevelState::TwoGold: return (this->TwoGoldTexture);
        case ELevelState::ThreeGold: return (this->ThreeGoldTexture);
        default: break;
    }
    return (this->LockTexture);
}

void UGeneralMenuUserWidget::UpdateSettingsButton(FInfoMenuButton& InfoButton, FText TextBlock, ESlateVisibility TextBlockVisibility,
    int32 CountIndex, UTexture2D* TextureBrush, FVector2D ButtonSize)
{
    InfoButton.TextBlock->SetText(TextBlock);
    InfoButton.TextBlock->SetVisibility(TextBlockVisibility);
    InfoButton.Button->SetIndexLevel(CountIndex);
    InfoButton.Button->SetVisibility(ESlateVisibility::Visible);

    FSlateBrush SlateBrush;
    SlateBrush.SetResourceObject(TextureBrush);

    FSlateSound SlateSound;
    SlateSound.SetResourceObject(this->SoundPressButton);

    FButtonStyle ButtonStyle;
    ButtonStyle.Pressed = SlateBrush;
    ButtonStyle.Normal = SlateBrush;
    ButtonStyle.Hovered = SlateBrush;

    ButtonStyle.SetPressedSound(SlateSound);
    InfoButton.Button->SetStyle(ButtonStyle);
    InfoButton.Button->GetCanvasPanelSlot()->SetSize(ButtonSize);
}

void UGeneralMenuUserWidget::UpdateMusicVolumeFromCheckBox(bool bIsChecked)
{
    GetPPGameInstance()->GetMusicClass()->Properties.Volume = (bIsChecked) ? GetPPGameInstance()->GetDefaultMusicVolume() : 0.0f;
}

void UGeneralMenuUserWidget::UpdateSoundVolumeFromCheckBox(bool bIsChecked)
{
    GetPPGameInstance()->GetSoundClass()->Properties.Volume = (bIsChecked) ? GetPPGameInstance()->GetDefaultSoundVolume() : 0.0f;
}
