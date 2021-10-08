// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamePlayMode.generated.h"

class ACameraPawn;
class AGridGeneratorActor;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGamePlayMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGamePlayMode();

    // Getting pointer on grid generator
    AGridGeneratorActor* GetGridGenerator() { return (this->GridGeneratorPlatform); }

protected:
    virtual void StartPlay() override;

private:
    AGridGeneratorActor* GridGeneratorPlatform;
    ACameraPawn* CameraPawn;
};
