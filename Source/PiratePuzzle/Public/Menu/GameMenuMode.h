// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuDataTypes.h"
#include "GameMenuMode.generated.h"

class UPPGameInstance;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGameMenuMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    AGameMenuMode();

    // Delegate signature for level change
    FOnMenuStateChangedSignature OnMenuStateChanged;

    // Function for changing the status in the menu
    UFUNCTION(BlueprintCallable, Category = "GameMenuMode")
    void ChangeMenuState(EMenuState NewState);

    // Function for changing the status in the menu on timer
    void ChangeMenuStateTimer(EMenuState NewState, float RateTime);

protected:
    virtual void StartPlay() override;

private:
    // Current status of the menu
    EMenuState MenuState = EMenuState::None;

    // Pointer of class UPPGameInstance
    UPPGameInstance* GameInstance;
};
