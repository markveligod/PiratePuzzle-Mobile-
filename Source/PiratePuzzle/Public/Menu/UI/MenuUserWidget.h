// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUserWidget.generated.h"

class UPPGameInstance;
class AGameMenuMode;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UMenuUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Is widget Active
    bool IsWidgetActive() const { return (this->bActiveWidget); }
    // Change widget activity
    virtual void ChangeActiveWidget(bool NewState) { this->bActiveWidget = NewState; }
    // Change widget activity on timer
    void ChangeActiveWidgetTimer(bool NewState, float RateTime);

    // Show Animation
    void ShowAnim(UWidgetAnimation* Anim) { PlayAnimation(Anim); }

    // Playing the animation on timer
    void PlayAnimationTimer(UWidgetAnimation* Anim, float RateTime);

    // Mandatory animations for changing the menu state
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* StartAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* EndAnim;

protected:
    // Getting current game menu mode
    AGameMenuMode* GetGameMenuMode() const { return (this->GameMode); }
    // Getting current game instance
    UPPGameInstance* GetPPGameInstance() const { return (this->GameInstance); }

    // Starting the level by timer
    void StartLevelTimer(FName NameLevel, float TimeRate);

    // Play sound on Timer
    void PlaySoundTimer(USoundBase* SoundToPlay, float RateTime);

    virtual void NativeOnInitialized() override;

private:
    // The current pointer to the game menu mode
    AGameMenuMode* GameMode;

    // The current pointer to the Game Instance
    UPPGameInstance* GameInstance;

    // Checking widget activity
    bool bActiveWidget = true;

    // Starting a level without a timer
    void StartLevel(FName NameLevel) const;
};
