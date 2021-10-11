#pragma once

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
