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
