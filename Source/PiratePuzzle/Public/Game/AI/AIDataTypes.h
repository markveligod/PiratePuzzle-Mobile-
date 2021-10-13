#pragma once

#include "AIDataTypes.generated.h"

// A special state for a skeleton with a cannon
UENUM(BlueprintType)
enum class EStateSkeletonCannon : uint8
{
    None = 0,
    Fire,
    Reload,
    Win,
    Lose
};

// State player ai character
UENUM(BlueprintType)
enum class EStateAI : uint8
{
    None = 0,
    Idle,
    Walk,
    Death,
    Win,
    Attack
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateChangeFinalAnim, EStateAI);

// A set of parameters for spawning a runner's skeleton
class ASkeletonRunnerCharacter;

USTRUCT(BlueprintType)
struct FInfoSpawnSkeletonRunner
{
    GENERATED_BODY()

    // Spawn location on the grid generator.
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ToolTip = "Spawn location on the grid generator."))
    FIntPoint SpawnLocation;
    // Array of positions as a route for the runner's skeleton
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ToolTip = "Array of positions as a route for the runner's skeleton."))
    TArray<FIntPoint> RoadRoute;
    // Turn the runner's skeleton before the start of the game.
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ToolTip = "Turn the runner's skeleton before the start of the game."))
    float RotAxisZ = 0.f;

    FInfoSpawnSkeletonRunner() {}
    FInfoSpawnSkeletonRunner(const FInfoSpawnSkeletonRunner& Other)
    {
        this->SpawnLocation = Other.SpawnLocation;
        this->RoadRoute = Other.RoadRoute;
        this->RotAxisZ = Other.RotAxisZ;
    }
};
