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

    // Getting pointer on camera pawn
    ACameraPawn* GetCameraPawn() const { return (this->CameraPawn); }

protected:
    virtual void StartPlay() override;

private:
    AGridGeneratorActor* GridGeneratorPlatform;
    ACameraPawn* CameraPawn;

    // Current Game State
    EGameState GameState = EGameState::WaitToStart;
};
