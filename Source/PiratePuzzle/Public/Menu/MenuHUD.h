// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "MenuDataTypes.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

class AGameMenuMode;
class UMenuUserWidget;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AMenuHUD : public AHUD
{
    GENERATED_BODY()

public:
protected:
    virtual void BeginPlay() override;

private:
    // Game mode pointer
    AGameMenuMode* GameMode = nullptr;

    // Map for storing all widgets
    TMap<EMenuState, UMenuUserWidget*> GameWidgets;
    UMenuUserWidget* WidgetOnVisible = nullptr;

    // A set of widgets that will be used to switch between level states on a delegate call
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Welcome widget UI from UMenuUserWidget"))
    TSubclassOf<UMenuUserWidget> WelcomeWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Menu widget UI from UMenuUserWidget"))
    TSubclassOf<UMenuUserWidget> MenuWidgetClass;

    // Callback function for changing widgets depending on the menu state
    void OnChangeMenuState(EMenuState NewState);

    // Timer visible and Hidden widget
    FTimerHandle TimerHandleCollapse;

    // Change on new widget
    UFUNCTION()
    void ChangeOnNewWidget(EMenuState NewState);
};
