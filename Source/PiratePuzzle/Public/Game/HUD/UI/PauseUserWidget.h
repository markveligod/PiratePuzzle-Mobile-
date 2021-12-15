// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "PauseUserWidget.generated.h"

class UCheckBox;
class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UPauseUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:
    // Check box settings
    UPROPERTY(Transient, meta = (BindWidget))
    UCheckBox* MusicCheckBox;
    UPROPERTY(Transient, meta = (BindWidget))
    UCheckBox* SoundCheckBox;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CloseButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseButtonAnim;

    // Home
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* HomeButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HomeButtonAnim;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* RestartButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* RestartButtonAnim;

    // Function for Restart game
    UFUNCTION()
    void OnClickedRestart();

    // Function for switching to the game state
    UFUNCTION()
    void OnClickedCloseWidget();

    // Home
    UFUNCTION()
    void OnClickedHome();

    // Call back function for update sound and music classes in FOnCheckBoxComponentStateChanged
    UFUNCTION()
    void UpdateMusicVolumeFromCheckBox(bool bIsChecked);
    UFUNCTION()
    void UpdateSoundVolumeFromCheckBox(bool bIsChecked);
};
