// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "PauseUserWidget.generated.h"

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
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CloseButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseButtonAnim;

    // Function for switching to the game state
    UFUNCTION()
    void OnClickedCloseWidget();
};
