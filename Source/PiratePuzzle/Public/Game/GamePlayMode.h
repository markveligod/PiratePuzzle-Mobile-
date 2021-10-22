// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameDataTypes.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayMode.generated.h"

class ASkeletonCannonPawn;
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

    // Getting status loading
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "GamePlayMode")
    FString GetStatusLoading() const;

    // Getting pointer on grid generator
    AGridGeneratorActor* GetGridGenerator() { return (this->GridGeneratorPlatform); }

    // Delegate change current game state
    FOnGameStateChangedSignature OnGameStateChanged;

    // A single delegate for sending information about
    FCoinIncreaseSignature OnCoinIncrease;

    // Change state function
    UFUNCTION(BlueprintCallable, Category = "GamePlayMode")
    void OnChangeGameState(EGameState NewState);
    // Change state with timer
    void OnChangeGameStateTimer(EGameState State, float Time = 0.f);

    // Getting pointer on camera pawn
    ACameraPawn* GetCameraPawn() const { return (this->CameraPawn); }

    // Getting current game play state
    UFUNCTION(BlueprintCallable, Category = "GamePlayMode")
    EGameState GetGameState() const { return (this->GameState); }

    // Reset from quicksand dead
    UFUNCTION(BlueprintCallable, Category = "GamePlayMode")
    void ResetDead();

    // The function of increasing the number of coins
    void IncreaseCountCoin();

    // Getting current number of coins
    UFUNCTION(BlueprintCallable, Category = "GamePlayMode")
    int32 GetCountCoin() const { return (this->CountCoin); }

protected:
    virtual void StartPlay() override;

private:
    // Pointer on AGridGeneratorActor class
    AGridGeneratorActor* GridGeneratorPlatform;
    // Pointer on ACameraPawn class
    ACameraPawn* CameraPawn;
    // Array pointers ASkeletonCannonPawn class
    TArray<ASkeletonCannonPawn*> ArraySkeletonCannon;

    // Status Loading
    bool bSpawnPlatform = false;
    bool bSpawnWall = false;
    bool bSpawnPirate = false;
    bool bSpawnSkeletonRunner = false;
    bool bSpawnGold = false;

    // Time after how long will the state change
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time after how long will the state change."))
    float RateTime = 1.5f;

    // Current Game State
    EGameState GameState = EGameState::WaitToStart;

    // The number of coins collected at the current level
    int32 CountCoin = 0;

    // Function to stop all running skeletons on the grid
    void StopAllSkeletonRunner();

    // Function to stop all Skeleton cannon on the level
    void StopAllSkeletonCannon();

    // Function to start all running skeletons on the grid
    void StartAllSkeletonRunner();

    // Function to start all Skeleton cannon on the level
    void StartAllSkeletonCannon();

    // Spawn platform
    void SpawnPlatform();
    // Spawn Wall
    void SpawnWall();
    // Spawn Pirate
    void SpawnPirate();
    // Spawn Skeleton Runner
    void SpawnSkeletonRunner();
    // Spawn Gold
    void SpawnGold();
};
