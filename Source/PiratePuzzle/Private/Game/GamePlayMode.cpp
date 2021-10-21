// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/GamePlayMode.h"

#include "Components/CapsuleComponent.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Camera/CameraPlayerController.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/GameHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePlayMode, All, All);

AGamePlayMode::AGamePlayMode()
{
    DefaultPawnClass = ACameraPawn::StaticClass();
    PlayerControllerClass = ACameraPlayerController::StaticClass();
    HUDClass = AGameHUD::StaticClass();
}

FString AGamePlayMode::GetStatusLoading() const
{
    if (this->bSpawnPlatform && !this->bSpawnWall && !this->bSpawnPirate && !this->bSpawnSkeletonRunner && !this->bSpawnGold)
    {
        return ("Spawn platform");
    }
    else if (this->bSpawnPlatform && this->bSpawnWall && !this->bSpawnPirate && !this->bSpawnSkeletonRunner && !this->bSpawnGold)
    {
        return ("Spawn Wall");
    }
    else if (this->bSpawnPlatform && this->bSpawnWall && this->bSpawnPirate && !this->bSpawnSkeletonRunner && !this->bSpawnGold)
    {
        return ("Spawn Pirate");
    }
    else if (this->bSpawnPlatform && this->bSpawnWall && this->bSpawnPirate && this->bSpawnSkeletonRunner && !this->bSpawnGold)
    {
        return ("Spawn Skeleton Runner");
    }
    else if (this->bSpawnPlatform && this->bSpawnWall && this->bSpawnPirate && this->bSpawnSkeletonRunner && this->bSpawnGold)
    {
        return ("Spawn Gold");
    }

    return ("Preparing for spawn");
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

    if (NewState == EGameState::GameOver || NewState == EGameState::GameWin)
    {
        this->StopAllSkeletonRunner();
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

    FTimerHandle TimerSpawnPlatform;
    FTimerHandle TimerSpawnWall;
    FTimerHandle TimerSpawnPirate;
    FTimerHandle TimerSpawnSkeletonRunner;
    FTimerHandle TimerSpawnGold;

    GetWorldTimerManager().SetTimer(TimerSpawnPlatform, this, &AGamePlayMode::SpawnPlatform, 1.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnWall, this, &AGamePlayMode::SpawnWall, 2.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnPirate, this, &AGamePlayMode::SpawnPirate, 3.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnSkeletonRunner, this, &AGamePlayMode::SpawnSkeletonRunner, 4.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnGold, this, &AGamePlayMode::SpawnGold, 5.f, false);

    this->OnChangeGameState(EGameState::Loading);
    this->OnChangeGameStateTimer(EGameState::StartInfo, 6.f);
}

void AGamePlayMode::StopAllSkeletonRunner()
{
    TArray<ASkeletonRunnerCharacter*> TempArraySkeletonRunners = this->GridGeneratorPlatform->GetArraySkeletonRunners();
    for (const auto TempRunner : TempArraySkeletonRunners)
    {
        TempRunner->SetNewStateAISkeletonRunner(EStateAI::None);
        TempRunner->GetCharacterMovement()->StopActiveMovement();
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("All skeleton runners is stop movement"));
}

void AGamePlayMode::SpawnPlatform()
{
    this->GridGeneratorPlatform->SpawnPlatform();
    this->bSpawnPlatform = true;
}

void AGamePlayMode::SpawnWall()
{
    this->GridGeneratorPlatform->SpawnWall();
    this->bSpawnWall = true;
}

void AGamePlayMode::SpawnPirate()
{
    this->GridGeneratorPlatform->SpawnPirate();
    this->bSpawnPirate = true;
}

void AGamePlayMode::SpawnSkeletonRunner()
{
    this->GridGeneratorPlatform->SpawnSkeletonRunners();
    this->bSpawnSkeletonRunner = true;
}

void AGamePlayMode::SpawnGold()
{
    this->GridGeneratorPlatform->SpawnGold();
    this->bSpawnGold = true;
}

void AGamePlayMode::ResetDead()
{
    APirateAICharacter* TempPirate = this->CameraPawn->GetAIPirate();
    FIntPoint LastPoint = TempPirate->GetLastPositionPoint();
    auto MapPlatform = this->GridGeneratorPlatform->GetMapPlatformOnGrid();
    FVector ResetToLocation = BaseUtils::GetVectorPositionPlatform(LastPoint, MapPlatform);
    ResetToLocation.Z += this->GridGeneratorPlatform->GetPosPirateAxisZ();
    if (ResetToLocation == FVector::ZeroVector)
    {
        UE_LOG(LogGamePlayMode, Error, TEXT("Platform doesn't exist in map %s"), *LastPoint.ToString());
        return;
    }
    // Reset Pirate
    TempPirate->SetActorLocation(ResetToLocation);
    TempPirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TempPirate->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    TempPirate->SetStateAI(EStateAI::Idle);
    TempPirate->SetNewPosPlayer(LastPoint);

    // Reset Skeleton runner
    auto ArraySkeletonRun = this->GridGeneratorPlatform->GetArraySkeletonRunners();
    for (const auto TempRunner : ArraySkeletonRun)
    {
        TempRunner->SetNewStateAISkeletonRunner(EStateAI::Walk);
    }

    this->OnChangeGameState(EGameState::GameProgress);
}

void AGamePlayMode::IncreaseCountCoin()
{
    this->CountCoin++;
    this->OnCoinIncrease.Execute(this->CountCoin);
}
