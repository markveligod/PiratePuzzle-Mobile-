// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "StartInfoUserWidget.generated.h"

class UTextBlock;
class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UStartInfoUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:
    // Info text block
    UPROPERTY(Transient, meta = (BindWidget))
    UTextBlock* InfoTextBlock;

    // Bind close button
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CloseButton;

    // Animation for close button
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseButtonAnim;

    // Lang button and animation
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* LangButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LangButtonAnim;

    // Close widget to state game progress
    UFUNCTION()
    void CloseWidget();

    // Change language on RU
    UFUNCTION()
    void SwipeLang();
};
