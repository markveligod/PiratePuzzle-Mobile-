// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "StartInfoUserWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UStartInfoUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    // Close button delay before change game state
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim",
        meta = (AllowPrivateAccess = "true", ToolTip = "Close button delay before change game state"))
    float DelayCloseButton = 0.5f;

    // Bind close button
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CloseButton;

    // Animation for close button
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseButtonAnim;

    virtual void NativeOnInitialized() override;

private:
    // Close widget to state game progress
    UFUNCTION()
    void CloseWidget();
};
