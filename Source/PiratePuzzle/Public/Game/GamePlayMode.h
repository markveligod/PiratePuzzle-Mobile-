// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameDataTypes.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayMode.generated.h"

class ACameraPawn;
class AGridGeneratorActor;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGamePlayMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGamePlayMode();

    // Getting pointer on grid generator
    AGridGeneratorActor* GetGridGenerator() { return (this->GridGeneratorPlatform); }

    // Delegate change current game state
    FOnGameStateChangedSignature OnGameStateChanged;

    // Change state function
    UFUNCTION(BlueprintCallable)
    void OnChangeGameState(EGameState NewState);
    // Change state with timer
    void OnChangeGameStateTimer(EGameState State, float Time = 0.f);

    // Getting pointer on camera pawn
    ACameraPawn* GetCameraPawn() const { return (this->CameraPawn); }

    // Getting current game play state
    UFUNCTION(BlueprintCallable)
    EGameState GetGameState() const { return (this->GameState); }

protected:
    virtual void StartPlay() override;

private:
    AGridGeneratorActor* GridGeneratorPlatform;
    ACameraPawn* CameraPawn;

    // Time after how long will the state change
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time after how long will the state change."))
    float RateTime = 1.5f;

    // Current Game State
    EGameState GameState = EGameState::WaitToStart;
};
