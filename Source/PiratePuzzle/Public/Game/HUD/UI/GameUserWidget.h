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
    UPROPERTY(meta = (BindWidget))
    UButton* CameraButton;
    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    // Function on Click swap camera
    UFUNCTION()
    void OnClickedSwapCamera();

    // Function on Click Back position pirate
    UFUNCTION()
    void OnClickedBackPos();
};
