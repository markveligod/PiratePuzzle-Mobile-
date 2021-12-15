// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/GameDataTypes.h"
#include "Game/HUD/DebugHUD.h"
#include "GameHUD.generated.h"

class AGamePlayMode;
class UBaseUserWidget;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGameHUD : public ADebugHUD
{
    GENERATED_BODY()
protected:
    // Draw the HUD.
    virtual void DrawHUD() override;

    //
    virtual void BeginPlay() override;

private:
    // Game mode pointer
    UPROPERTY()
    AGamePlayMode* GameMode = nullptr;

    // Map for storing all widgets
    TMap<EGameState, UBaseUserWidget*> GameWidgets;
    UPROPERTY()
    UBaseUserWidget* WidgetOnVisible = nullptr;

    // A set of widgets that will be used to switch between level states on a delegate call
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Start Info widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> StartInfoWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Game widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> GameWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Pause widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> PauseWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Game Win widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> GameWinWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Game Over widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> GameOverWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Loading widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> LoadingWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Ads count widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> AdsCountWidgetClass;

    // Callback function for changing widgets depending on the game state
    void OnChangeGameState(EGameState NewState);

    // Timer visible and Hidden widget
    FTimerHandle TimerHandleCollapse;

    // Change on new widget
    UFUNCTION()
    void ChangeOnNewWidget(EGameState NewState);
};
