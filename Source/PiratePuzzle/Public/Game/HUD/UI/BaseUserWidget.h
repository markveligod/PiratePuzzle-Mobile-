// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/GameDataTypes.h"
#include "BaseUserWidget.generated.h"

class AGamePlayMode;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UBaseUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Play the animation
    UFUNCTION()
    void ShowAnim(UWidgetAnimation* Anim);
    // Play the animation with Timer
    void ShowAnimTimer(UWidgetAnimation* Anim, float RateTime);

    // Getting start and end animation rate time
    float GetRateTimeStartAnim() const { return (this->RateTimeStartAnim); }
    float GetRateTimeEndAnim() const { return (this->RateTimeEndAnim); }

    // Getting start and end animation
    UWidgetAnimation* GetStartAnim() const { return (this->StartAnim); }
    UWidgetAnimation* GetEndAnim() const { return (this->EndAnim); }

    // Enable button active
    UFUNCTION()
    void EnableButtonActive() { this->bIsButtonActive = true; }
    // Disable button active
    UFUNCTION()
    void DisableButtonActive() { this->bIsButtonActive = false; }
    // Timer state Button Active change function
    void ButtonActiveTimer(bool State, float RateTime);
    // State Active button
    bool GetStateActiveButton() const { return (this->bIsButtonActive); }

protected:
    // required animations for all child classes Start and End animation
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* StartAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* EndAnim;

    // Change Game state with timer
    void ChangeGameStateTimer(EGameState NewState, float RateTime);

    // Init
    virtual void NativeOnInitialized() override;
    // Getting Current Game mode
    AGamePlayMode* GetGamePlayMode() const { return (this->GamePlayMode); }

    // Close button delay before change game state
    UPROPERTY(
        EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim", meta = (ToolTip = "Close button delay before change game state"))
    float DelayCloseButton = 0.3f;

private:
    // State Activided buttons
    bool bIsButtonActive = true;

    // Timer to play the animation at a given time
    FTimerHandle TimerDelayCallBack;

    // Pointer on Current class AGamePlayMode
    AGamePlayMode* GamePlayMode;

    // Mandatory setting of the exact time animation playback Start and End
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim",
        meta = (AllowPrivateAccess = "true", ToolTip = "Specify the start animation time"))
    float RateTimeStartAnim = 0.5f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim",
        meta = (AllowPrivateAccess = "true", ToolTip = "Specify the end animation time"))
    float RateTimeEndAnim = 0.5f;
};
