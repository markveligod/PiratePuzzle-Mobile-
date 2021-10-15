// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/GamePlayMode.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Camera/CameraPlayerController.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePlayMode, All, All);

AGamePlayMode::AGamePlayMode()
{
    DefaultPawnClass = ACameraPawn::StaticClass();
    PlayerControllerClass = ACameraPlayerController::StaticClass();
    HUDClass = AGameHUD::StaticClass();
}

void AGamePlayMode::OnChangeGameState(EGameState NewState)
{
    UE_LOG(LogGamePlayMode, Display, TEXT("Change new state on %s"), *UEnum::GetValueAsString(NewState));
    if (NewState == this->GameState)
    {
        UE_LOG(LogGamePlayMode, Warning, TEXT("Current state: %s equal New State: %s | return ... "),
            *UEnum::GetValueAsString(this->GameState), *UEnum::GetValueAsString(NewState));
        return;
    }

    this->GameState = NewState;
    this->OnGameStateChanged.Broadcast(NewState);
}

void AGamePlayMode::OnChangeGameStateTimer(EGameState State, float Time)
{
    float TempTime = (Time == 0.f) ? this->RateTime : Time;
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, "OnChangeGameState", State);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TempTime, false);
}

void AGamePlayMode::StartPlay()
{
    Super::StartPlay();
    this->GridGeneratorPlatform =
        Cast<AGridGeneratorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridGeneratorActor::StaticClass()));
    checkf(this->GridGeneratorPlatform, TEXT("Grid is nullptr"));
    this->CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
    checkf(this->CameraPawn, TEXT("Camera pawn is nullptr"));

    this->OnChangeGameState(EGameState::StartInfo);
}
