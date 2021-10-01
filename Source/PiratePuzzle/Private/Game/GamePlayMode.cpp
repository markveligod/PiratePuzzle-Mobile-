// Copyright Epic Games, Inc. All Rights Reserved.


#include "Game/GamePlayMode.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Camera/CameraPlayerController.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Kismet/GameplayStatics.h"

AGamePlayMode::AGamePlayMode()
{
    DefaultPawnClass = ACameraPawn::StaticClass();
    PlayerControllerClass = ACameraPlayerController::StaticClass();
}

void AGamePlayMode::StartPlay()
{
    Super::StartPlay();
    this->GridGeneratorPlatform = Cast<AGridGeneratorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridGeneratorActor::StaticClass()));
    checkf(this->GridGeneratorPlatform, TEXT("Grid is nullptr"));
    this->CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
    checkf(this->CameraPawn, TEXT("Camera pawn is nullptr"));
}

