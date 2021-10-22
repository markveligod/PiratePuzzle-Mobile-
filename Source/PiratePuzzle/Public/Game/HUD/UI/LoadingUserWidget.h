// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "LoadingUserWidget.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API ULoadingUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()
public:
    virtual void EnableButtonActive() override;
    virtual void DisableButtonActive() override;

protected:
    virtual void NativeOnInitialized() override;

private:
    // The widget on which the image will be changed
    UPROPERTY(meta = (BindWidget))
    UImage* LoadingImage;

    // Array of picture samples
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "Array of picture samples"))
    TArray<UTexture2D*> ArrayPictureSamples;
    // The time interval for calling the function again
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Widget",
        meta = (AllowPrivateAccess = true, ToolTip = "The time interval for calling the function again"))
    float TimeRate = 0.1f;
    // The size of the image to install in the widget
    FVector2D ImageSize;
    // Current count in ArrayPictureSamples
    int32 CountPicture = 0;

    // The function of changing the image from the ArrayPictureSamples
    void ChangeLoadingImage();

    // Timer Handle
    FTimerHandle TimerChangeImage;
};
