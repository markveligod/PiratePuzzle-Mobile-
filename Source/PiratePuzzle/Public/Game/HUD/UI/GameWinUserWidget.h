// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/GameDataTypes.h"
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
    UPROPERTY(EditDefaultsOnly, Category = "Settings Widget");
    TAssetPtr<UTexture2D> GoldTexture;
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

    // Home button and Anim
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* HomeButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HomeButtonAnim;

    // Home button and Anim
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* NextLevelButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* NextLevelButtonAnim;

    // Image coin button and Anim
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* OpenCoinImage;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ImageCoinAnim;

    // Restart game
    UFUNCTION()
    void OnClickedRestart();

    // Function for tracking coin changes
    void ChangeCountCoin(int32 CountCoin);

    // Home
    UFUNCTION()
    void OnClickedHome();

    // Next level
    UFUNCTION()
    void OnClickedNextLevel();

    // Game state
    void OnChangeGameState(EGameState NewGameState);
};
