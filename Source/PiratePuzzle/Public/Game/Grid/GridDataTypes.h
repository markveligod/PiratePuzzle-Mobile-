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
