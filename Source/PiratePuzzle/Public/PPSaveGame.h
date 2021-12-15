// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Menu/MenuDataTypes.h"
#include "PPSaveGame.generated.h"

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UPPSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // map data levels on save
    UPROPERTY(VisibleAnywhere, Category = Basic)
    TMap<int32, FInfoLevel> MapSavedLevels;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    int32 SaveCountMoveBack;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MusicVolume;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float SoundVolume;
};
