// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameDataTypes.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayMode.generated.h"

class USoundCue;
class UNiagaraSystem;
class UNiagaraComponent;
class UPPGameInstance;
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

    // Event fps display in one second
    FSendFPSOnSecondSignature FPSOnSecondDelegate;

    // Getting pointer on grid generator
    AGridGeneratorActor* GetGridGenerator() const { return (this->GridGeneratorPlatform); }

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

    // Add one success moves per
    void AddCountMoves() { this->CountMoves++; }

    // Spawn Effect
    void SpawnEffect(UNiagaraSystem* Effect, FVector Location);

    // Spawn Effect on timer
    void SpawnEffectTimer(UNiagaraSystem* Effect, FVector Location, float RateTimeEffect = 0.0f);

    // Function to stop all running skeletons on the grid
    void StopAllSkeletonRunner();

    // Function to stop all Skeleton cannon on the level
    void StopAllSkeletonCannon();

    // Function to start all running skeletons on the grid
    void StartAllSkeletonRunner();

    // Function to start all Skeleton cannon on the level
    void StartAllSkeletonCannon();

protected:
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    // Effect teleport OUT
    UPROPERTY(EditDefaultsOnly, Category = "Settings Widget")
    UNiagaraSystem* TeleportOutEffect;

    // Settings background music
    UPROPERTY(EditDefaultsOnly, Category = "Settings sound")
    USoundCue* GameBGSound;
    // Settings background water sound
    UPROPERTY(EditDefaultsOnly, Category = "Settings sound")
    USoundCue* WaterSound;

    // Rate time loading level
    UPROPERTY(EditDefaultsOnly, Category = "Settings Loading")
    float RateTimeLoading = 0.5f;
    // Current pointer on Game user settings
    UPROPERTY()
    UGameUserSettings* GameSettings;
    // Pointer to a class PPGameInstance
    UPROPERTY()
    UPPGameInstance* GameInstance;
    // Pointer on AGridGeneratorActor class
    UPROPERTY()
    AGridGeneratorActor* GridGeneratorPlatform;
    // Pointer on ACameraPawn class
    UPROPERTY()
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

    // Parameters for calculating fps
    UPROPERTY(EditDefaultsOnly, Category = "Settings FPS",
        meta = (ToolTip = "The lower edge for reducing the quality of the render", ClampMin = "30", ClampMax = "100"))
    int32 LowerEdgeFPS = 45;
    UPROPERTY(EditDefaultsOnly, Category = "Settings FPS",
        meta = (ToolTip = "The upper edge to improve the quality of the render", ClampMin = "30", ClampMax = "100"))
    int32 UpperEdgeFPS = 60;
    float DeltaTimeOneSec = 0.0f;
    int32 CountFPS = 0;
    int32 MaxCountFPS = 0;

    // Function for saving data
    void SavingData() const;

    // Time from the start of the level to its end (sec)
    float TotalTimeLevel = 0.0f;

    // Current Game State
    EGameState GameState = EGameState::WaitToStart;

    // Number of successful moves per level
    int32 CountMoves = 0;

    // The number of coins collected at the current level
    int32 CountCoin = 0;

    // Setup all settings new preset
    void SetupPresetSettings(int32 FPS);

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

    friend class AGameHUD;
};
