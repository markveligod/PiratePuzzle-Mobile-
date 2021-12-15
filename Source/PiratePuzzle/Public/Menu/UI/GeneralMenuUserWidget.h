// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Menu/MenuDataTypes.h"
#include "Menu/UI/MenuUserWidget.h"
#include "GeneralMenuUserWidget.generated.h"

class USoundCue;
class UImage;
class UMenuButton;
class UTextBlock;
class UButton;
class UCheckBox;

USTRUCT()
struct FInfoMenuButton
{
    GENERATED_BODY()

    // Pointer to the button position
    UPROPERTY()
    UMenuButton* Button = nullptr;
    // A pointer to its text block
    UPROPERTY()
    UTextBlock* TextBlock = nullptr;
    // Animation for a click event
    UPROPERTY()
    UWidgetAnimation* AnimPlay = nullptr;

    FInfoMenuButton() {}
    FInfoMenuButton(UMenuButton* NewButton, UTextBlock* NewTextBlock, UWidgetAnimation* NewAnimPlay)
    {
        this->Button = NewButton;
        this->TextBlock = NewTextBlock;
        this->AnimPlay = NewAnimPlay;
    }
    FInfoMenuButton(const FInfoMenuButton& Other)
    {
        this->Button = Other.Button;
        this->TextBlock = Other.TextBlock;
        this->AnimPlay = Other.AnimPlay;
    }
    void operator=(const FInfoMenuButton& Other)
    {
        this->Button = Other.Button;
        this->TextBlock = Other.TextBlock;
        this->AnimPlay = Other.AnimPlay;
    }
};

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UGeneralMenuUserWidget : public UMenuUserWidget
{
    GENERATED_BODY()
public:
    virtual void ChangeActiveWidget(bool NewState) override;

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

private:
    // Sound cue for music background
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Sound cue for music background"))
    USoundCue* MusicBg;
    // Play rate time music background
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Play rate time music background"))
    float PlayBgRateTime = 2.f;
    // Sound cue for press button
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Sound cue for press button"))
    USoundCue* SoundPressButton;

    // A set of images for changing the state of the level
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture with zero gold coins"))
    UTexture2D* ZeroGoldTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture with one gold coins"))
    UTexture2D* OneGoldTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture with two gold coins"))
    UTexture2D* TwoGoldTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture with three gold coins"))
    UTexture2D* ThreeGoldTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true, ToolTip = "Texture with lock"))
    UTexture2D* LockTexture;
    // Button Size for other texture
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Button Size for other texture"))
    FVector2D ButtonSizeOther;
    // Button Size for lock texture
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings",
        meta = (AllowPrivateAccess = true, ToolTip = "Button Size for lock texture"))
    FVector2D ButtonSizeLock;

    // The crutch variable to start the animation AnimPlayButton
    bool bAnimPlayButton = false;

    // Part Start overlay
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* PlayButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimPlayButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LoopWheelImage;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimUpStartOverlay;

    // Next and Prev button, text blocks and anim
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimNextButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimPrevButton;
    UPROPERTY(meta = (BindWidget))
    UButton* NextButton;
    UPROPERTY(meta = (BindWidget))
    UButton* PrevButton;
    UPROPERTY(meta = (BindWidget))
    UImage* BackgroundImage;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaxTextBlock;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MinTextBlock;

    // Position levels
    UPROPERTY(meta = (BindWidget))
    UMenuButton* FirstPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* FirstPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* SecondPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SecondPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* ThirdPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ThirdPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* FourthPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* FourthPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* FifthPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* FifthPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* SixthPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SixthPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* SeventhPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SeventhPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* EighthPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* EighthPositionTextBlock;

    UPROPERTY(meta = (BindWidget))
    UMenuButton* NinthPosition;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* NinthPositionTextBlock;

    // Animation buttons
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimFirstButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimSecondButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimThirdButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimFourthButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimFifthButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimSixthButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimSeventhButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimEighthButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimNinthButton;

    // part Settings Overlay
    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;
    UPROPERTY(meta = (BindWidget))
    UButton* CloseSettingsButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimSettingsButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimCloseSettingsButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimVisibleSettingsOverlay;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* AnimHiddenSettingsOverlay;

    // Check box settings
    UPROPERTY(Transient, meta = (BindWidget))
    UCheckBox* MusicCheckBox;
    UPROPERTY(Transient, meta = (BindWidget))
    UCheckBox* SoundCheckBox;

    // Key interval in the level map
    TInterval<int32> RangeLevels;

    // Map All levels
    TMap<int32, FInfoLevel> MapAllLevels;

    // Array with information about the button
    TArray<FInfoMenuButton> ArrayInfoMenuButtons;

    // Constant value for changing the list
    const int32 CountList = 9;

    // Function change to display levels
    UFUNCTION()
    void OnClickedVisibleLevels();

    // Function change to display settings
    UFUNCTION()
    void VisibleSettings();
    UFUNCTION()
    void HiddenSettings();

    UFUNCTION()
    void ChangeVisibleButtons(ESlateVisibility NewVis);

    // Function to call the following list of levels
    UFUNCTION()
    void SetNextListButtons();

    // Function for calling the previous list of levels
    UFUNCTION()
    void SetPrevListButtons();

    // Function for a callback with a level change
    UFUNCTION()
    void StartLevel(int32 IndexLevel, UWidgetAnimation* AnimToPlayButton);

    // The function of updating configurations of positions of levels on the map
    void UpdateConfigurationMap();

    // Return the picture depending on the condition
    UTexture2D* GetPictureFromLevelState(ELevelState LevelState) const;

    // Function for updating the insides of the menu button
    void UpdateSettingsButton(FInfoMenuButton& InfoButton, FText TextBlock, ESlateVisibility TextBlockVisibility, int32 CountIndex,
        UTexture2D* TextureBrush, FVector2D ButtonSize);

    // Call back function for update sound and music classes in FOnCheckBoxComponentStateChanged
    UFUNCTION()
    void UpdateMusicVolumeFromCheckBox(bool bIsChecked);
    UFUNCTION()
    void UpdateSoundVolumeFromCheckBox(bool bIsChecked);
};
