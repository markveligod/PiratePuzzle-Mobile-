// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "Game/GameDataTypes.h"
#include "GameUserWidget.generated.h"

class UImage;
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
    // Image of a gold coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget", meta = (AllowPrivateAccess = true, ToolTip = "Image of a gold coin"));
    UTexture2D* GoldTexture;
    // Texture size image of a gold coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget", meta = (AllowPrivateAccess = true, ToolTip = "Texture size image of a gold coin"));
    FVector2D SizeGoldTexture;
    
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CameraButton;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* BackButton;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* PauseButton;
    
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* ImageOneCoin;
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* ImageTwoCoin;
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* ImageThreeCoin;
    

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HelpButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* PauseButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* CameraButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* BackButtonAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ImageOneCoinAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ImageTwoCoinAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* ImageThreeCoinAnim;

    // Function on Click swap camera
    UFUNCTION()
    void OnClickedSwapCamera();

    // Function on Click Back position pirate
    UFUNCTION()
    void OnClickedBackPos();

    // Function on Click Pause state
    UFUNCTION()
    void OnClickedPauseState();

    // Function for subscribing to the delegate of the coin replenishment event
    void ChangeCountCoin(int32 CountCoin);
};
