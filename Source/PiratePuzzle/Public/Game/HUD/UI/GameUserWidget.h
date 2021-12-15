// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameUserWidget.generated.h"

class USoundCue;
class UNiagaraSystem;
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
    // Effect teleport in
    UPROPERTY(EditDefaultsOnly, Category = "Settings Teleport")
    UNiagaraSystem* TeleportInEffect;
    // Effect teleport out
    UPROPERTY(EditDefaultsOnly, Category = "Settings Teleport")
    UNiagaraSystem* TeleportOutEffect;
    // Rate time effect teleport IN
    UPROPERTY(EditDefaultsOnly, Category = "Settings Teleport")
    float RateTimeTeleportIn = 0.5f;

    // Image of a gold coin
    UPROPERTY(EditDefaultsOnly, Category = "Settings Widget");
    TAssetPtr<UTexture2D> GoldTexture;
    // Texture size image of a gold coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture size image of a gold coin"));
    FVector2D SizeGoldTexture;
    // Set of textures for the BackButton
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Set of textures for the BackMoveButton"));
    TArray<TAssetPtr<UTexture2D>> BackMoveTexture;
    // Texture size image for the BackButton
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture size image for the BackMoveButton"));
    FVector2D SizeBackTexture;
    // Sound teleport start
    UPROPERTY(EditDefaultsOnly, Category = "Settings Widget")
    USoundCue* TeleportStartSound;

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

    // Anim montage pirate class to Fly
    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* AnimFlyPirate;
    // Anim montage pirate class to Jump
    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* AnimJumpPirate;
    // Anim montage pirate class to Stand
    UPROPERTY(EditDefaultsOnly)
    UAnimMontage* AnimStandPirate;

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

    // Function for update text block count add
    void UpdateTextCount(int32 NewCount);

    // Teleport to location
    void TeleportObject(APiratePawn* TempPirate, FVector NewLoc, FIntPoint NewPoint);
};
