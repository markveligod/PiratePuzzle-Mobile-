#pragma once

#include "Math/Interval.h"
#include "GameDataTypes.generated.h"

// game state structure for easy switching between widgets
UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitToStart = 0,
    StartInfo,
    GameProgress,
    Pause,
    Help,
    GameWin,
    GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);

// Own interval
USTRUCT(BlueprintType)
struct FIntervalFloat : public FFloatInterval
{
    GENERATED_BODY();

    FIntervalFloat() {}
    FIntervalFloat(float Max, float Min) : FFloatInterval(Max, Min) {}
};

// A single delegate for sending information about
DECLARE_DELEGATE_OneParam(FCoinIncreaseSignature, int32);
