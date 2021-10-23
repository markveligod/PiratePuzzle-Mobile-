// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUserWidget.generated.h"

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
    void ChangeActiveWidget(bool NewState) { this->bActiveWidget = NewState; }
    // Change widget activity on timer
    void ChangeActiveWidgetTimer(bool NewState, float RateTime);

    // Show Animation
    void ShowAnim(UWidgetAnimation* Anim) { PlayAnimation(Anim); }

    // Playing the animation on timer
    void PlayAnimationTimer(UWidgetAnimation* Anim, float RateTime);

    // Mandatory animations for changing the menu state
    UPROPERTY(meta = (BindWidget))
    UWidgetAnimation* StartAnim;
    UPROPERTY(meta = (BindWidget))
    UWidgetAnimation* EndAnim;

protected:
    // Getting game menu mode
    AGameMenuMode* GetGameMenuMode() const { return (this->GameMode); }

    virtual void NativeOnInitialized() override;

private:
    // The current pointer to the game menu mode
    AGameMenuMode* GameMode;

    // Checking widget activity
    bool bActiveWidget = true;
};
