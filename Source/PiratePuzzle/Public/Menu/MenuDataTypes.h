#pragma once

#include "MenuDataTypes.generated.h"

// Menu state structure for easy switching between widgets
UENUM(BlueprintType)
enum class EMenuState : uint8
{
    None = 0,
    Welcome,
    MenuBase
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMenuStateChangedSignature, EMenuState);

// Level status
UENUM(BlueprintType)
enum class ELevelState : uint8
{
    None = 0,
    Lock,
    ZeroGold,
    OneGold,
    TwoGold,
    ThreeGold
};

USTRUCT(BlueprintType)
struct FInfoLevel
{
    GENERATED_BODY()

    // The name of the level in the content browser
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The name of the level in the content browser"))
    FName NameLevel = NAME_None;
    // Level status
    UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Level status"))
    ELevelState LevelState = ELevelState::None;
    // The number of coins collected at the level
    UPROPERTY(VisibleDefaultsOnly, meta = (ToolTip = "The number of coins collected at the level"))
    int32 CountCoin = 0;

    FInfoLevel() {}
    FInfoLevel(const FInfoLevel& Other)
    {
        this->NameLevel = Other.NameLevel;
        this->LevelState = Other.LevelState;
        this->CountCoin = Other.CountCoin;
    }
    void operator=(const FInfoLevel& Other)
    {
        this->NameLevel = Other.NameLevel;
        this->LevelState = Other.LevelState;
        this->CountCoin = Other.CountCoin;
    }
};
