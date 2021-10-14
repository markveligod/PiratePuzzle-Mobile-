// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameUserWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UGameUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CameraButton;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* BackButton;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* PauseButton;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HelpButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* PauseButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CameraButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* BackButtonAnim;

    // Function on Click swap camera
    UFUNCTION()
    void OnClickedSwapCamera();

    // Function on Click Back position pirate
    UFUNCTION()
    void OnClickedBackPos();

    // Function on Click Pause state
    UFUNCTION()
    void OnClickedPauseState();
};
