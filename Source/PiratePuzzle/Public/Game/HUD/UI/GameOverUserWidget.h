// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameOverUserWidget.generated.h"

class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UGameOverUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* RestartButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* RestartButtonAnim;

    // Restart game
    UFUNCTION()
    void OnClickedRestart();
};
