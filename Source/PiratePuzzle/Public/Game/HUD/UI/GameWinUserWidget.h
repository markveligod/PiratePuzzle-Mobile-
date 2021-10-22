// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameWinUserWidget.generated.h"

class UImage;
class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UGameWinUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

private:
    // Image of a gold coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Image of a gold coin"));
    UTexture2D* GoldTexture;
    // Texture size image of a gold coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture size image of a gold coin"));
    FVector2D SizeGoldTexture;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* RestartButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* RestartButtonAnim;

    UPROPERTY(Transient, meta = (BindWidget))
    UImage* CoinOneImage;
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* CoinTwoImage;
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* CoinThreeImage;

    // Restart game
    UFUNCTION()
    void OnClickedRestart();

    // Function for tracking coin changes
    void ChangeCountCoin(int32 CountCoin);
};
