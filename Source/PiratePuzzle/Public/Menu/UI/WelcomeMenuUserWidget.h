// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Menu/UI/MenuUserWidget.h"
#include "WelcomeMenuUserWidget.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UWelcomeMenuUserWidget : public UMenuUserWidget
{
    GENERATED_BODY()

public:
    virtual void ChangeActiveWidget(bool NewState) override;

protected:
    virtual void NativeOnInitialized() override;

private:
    UPROPERTY(Transient, meta = (BindWidget))
    UImage* WerfestLogoImage;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* WelcomeAnimation;
};
