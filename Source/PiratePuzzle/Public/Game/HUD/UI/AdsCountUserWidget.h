// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "PlayVideoProxy.h"
#include "AdsCountUserWidget.generated.h"

class UTextBlock;
class UButton;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UAdsCountUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    // Play video proxy
    UPlayVideoProxy* PlayVideoProxy = nullptr;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* AdsButton;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* CloseButton;

    UPROPERTY(Transient, meta = (BindWidget))
    UTextBlock* TimeAdTextBlock;

    // Function for viewing ads
    UFUNCTION()
    void ViewAds();

    // Function for close widget
    UFUNCTION()
    void CloseWidget();

    // Success view ads
    UFUNCTION()
    void PlayVideoSuccess(FString Item, int32 Amount);

    // Text render for timer view ads
    void ViewTimeAd() const;
};
