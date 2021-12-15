// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "PlayVideoProxy.h"
#include "Game/AI/AIDataTypes.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameOverUserWidget.generated.h"

class UTextBlock;
class UNiagaraSystem;
class UImage;
class UButton;
class UMaterialInstance;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UGameOverUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    UPROPERTY(Transient, meta = (BindWidget))
    UTextBlock* TimeAdTextBlock;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* RestartButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* RestartButtonAnim;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* BackButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* MoveBackButtonAnim;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* HomeButton;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* HomeButtonAnim;
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* ImageFlipBook;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    FVector2D DefaultSizeMaterial;
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInstance* LoseQuickSandMaterial;
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInstance* LoseSkeletonSandMaterial;
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInstance* LoseCannonSandMaterial;

    // Set of textures for the BackButton
    UPROPERTY(EditDefaultsOnly, Category = "Settings Widget");
    TArray<TAssetPtr<UTexture2D>> BackMoveTexture;
    // Texture size image for the BackButton
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Texture size image for the BackMoveButton"));
    FVector2D SizeBackTexture;

    //
    FDelegateHandle GameStateDelegateHandle;

    // Restart game
    UFUNCTION()
    void OnClickedRestart();

    // Ads game
    UFUNCTION()
    void OnClickedAds();

    // Home
    UFUNCTION()
    void OnClickedHome();

    // Signature for changing the game state
    void OnChangeGameState(EGameState NewState);

    // Signature function for changing pirate brains
    void OnChangeStateBrainPirate(EStateBrain NewStateBrain);

    // Function for update move back count add
    void UpdateImageMoveBackCount(int32 NewCount);

    // Success view ads
    UFUNCTION()
    void PlayVideoSuccess(FString Item, int32 Amount);

    // Text render for timer view ads
    void ViewTimeAd() const;
};
