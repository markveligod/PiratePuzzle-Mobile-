#pragma once

#include "Math/Interval.h"
#include "GameDataTypes.generated.h"

// game state structure for easy switching between widgets
UENUM(BlueprintType)
enum class EGameState : uint8
{
    WaitToStart = 0,
    Loading,
    StartInfo,
    GameProgress,
    Pause,
    Help,
    GameWin,
    GameOver,
    AdsSkill
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EGameState);

// Own interval
USTRUCT(BlueprintType)
struct FIntervalFloat
{
    GENERATED_BODY();

    UPROPERTY(EditInstanceOnly)
    float Min = 1.1f;
    UPROPERTY(EditInstanceOnly)
    float Max = 1.5f;

    FIntervalFloat() {}
    FIntervalFloat(float NewMin, float NewMax)
    {
        this->Min = NewMin;
        this->Max = NewMax;
    }
    FIntervalFloat(const FIntervalFloat& Other) { *this = Other; }
    void operator=(const FIntervalFloat& Other)
    {
        this->Min = Other.Min;
        this->Max = Other.Max;
    }
};

// A single delegate for sending information about
DECLARE_MULTICAST_DELEGATE_OneParam(FCoinIncreaseSignature, int32);

// Signature on the delegate to send the number of FPS in one second of time
DECLARE_MULTICAST_DELEGATE_OneParam(FSendFPSOnSecondSignature, int32);
