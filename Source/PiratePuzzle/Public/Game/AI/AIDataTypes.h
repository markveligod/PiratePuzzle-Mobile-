#pragma once

#include "Game/GameDataTypes.h"
#include "AIDataTypes.generated.h"

// A special state for a skeleton with a cannon
UENUM(BlueprintType)
enum class EStateBrain : uint8
{
    None = 0,
    Idle,
    Walk,
    Sand,
    FellCannon,
    FellSkeleton,
    Lose,
    Win,
    Attack,
    Fire,
    Reload
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateBrainChangedSignature, EStateBrain)

    // A set of parameters for spawning a runner's skeleton

    USTRUCT(BlueprintType) struct FInfoSpawnSkeletonRunner
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

    // Range reset timer to walk
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (ToolTip = "Range reset timer to walk."))
    FIntervalFloat RangeStopMove;

    FInfoSpawnSkeletonRunner() {}
    FInfoSpawnSkeletonRunner(const FInfoSpawnSkeletonRunner& Other) { *this = Other; }

    void operator=(const FInfoSpawnSkeletonRunner& Other)
    {
        this->SpawnLocation = Other.SpawnLocation;
        this->RoadRoute = Other.RoadRoute;
        this->RotAxisZ = Other.RotAxisZ;
        this->RangeStopMove = Other.RangeStopMove;
    }
};
