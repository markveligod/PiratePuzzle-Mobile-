// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
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
    AGamePlayMode* GameMode = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI",
        meta = (AllowPrivateAccess = "true", ToolTip = "Game widget UI from UBaseUserWidget"))
    TSubclassOf<UBaseUserWidget> GameWidgetClass;
};
