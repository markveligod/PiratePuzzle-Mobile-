// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "MenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMenuClickEvent, int32, InfoLevel, UWidgetAnimation*, Anim);

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UMenuButton : public UButton
{
    GENERATED_BODY()

public:
    // Setting new level information
    void SetIndexLevel(int32 NewIndex) { this->IndexLevel = NewIndex; }
    // Setting Animation play to button
    void SetPlayAnim(UWidgetAnimation* AnimPlay) { this->AnimToPlay = AnimPlay; }

    UPROPERTY(BlueprintAssignable, Category = "Button|Event")
    FOnMenuClickEvent OnMenuClick;

    UCanvasPanelSlot* GetCanvasPanelSlot() const { return (this->CanvasPanelSlot); }
    void SetCanvasPanelSlot(UCanvasPanelSlot* NewSlot) { this->CanvasPanelSlot = NewSlot; }

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

    FReply MenuSlateHandleClicked();

private:
    // The level information returned when the delegate is called
    int32 IndexLevel;

    // Pointer to the animation that will be played when clicked
    UWidgetAnimation* AnimToPlay = nullptr;

    // Pointer on current Canvas panel slot
    UCanvasPanelSlot* CanvasPanelSlot = nullptr;
};
