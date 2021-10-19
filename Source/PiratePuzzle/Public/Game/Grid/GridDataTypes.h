#pragma once

#include "GridDataTypes.generated.h"

UENUM(BlueprintType)
enum class EStatePlatform : uint8
{
    Neutral = 0,
    Barrier,
    Quicksand,
    Treasure
};

UENUM(BlueprintType)
enum class EStateLoading : uint8
{
    None = 0,
    SpawnPlatform,
    SpawnWall,
    SpawnPirate,
    SpawnSkeletonRunner,
    SpawnGold
};
