#pragma once

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