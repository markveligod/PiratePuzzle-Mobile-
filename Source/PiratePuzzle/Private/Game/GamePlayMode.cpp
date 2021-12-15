// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/GamePlayMode.h"
#include "GameAnalytics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "PPGameInstance.h"
#include "Components/BoxComponent.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/AI/SkeletonCannon/SkeletonCannonPawn.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerPawn.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Camera/CameraPlayerController.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/GameHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGamePlayMode, All, All);

AGamePlayMode::AGamePlayMode()
{
    PrimaryActorTick.bCanEverTick = true;

    DefaultPawnClass = ACameraPawn::StaticClass();
    PlayerControllerClass = ACameraPlayerController::StaticClass();
    HUDClass = AGameHUD::StaticClass();
}

void AGamePlayMode::OnChangeGameState(EGameState NewState)
{
    if (NewState == this->GameState)
    {
        UE_LOG(LogGamePlayMode, Warning, TEXT("Current state: %s equal New State: %s | return ... "),
            *UEnum::GetValueAsString(this->GameState), *UEnum::GetValueAsString(NewState));
        return;
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("Change new state on %s"), *UEnum::GetValueAsString(NewState));

    if (NewState != EGameState::GameProgress)
    {
        this->StopAllSkeletonRunner();
        this->StopAllSkeletonCannon();
    }

    if (NewState == EGameState::GameProgress)
    {
        this->StartAllSkeletonRunner();
        this->StartAllSkeletonCannon();
    }

    if (NewState == EGameState::GameWin)
    {
        // [Game Analytics] Sending total time on level
        UGameAnalytics::AddDesignEventWithValue("TotalTimeLevel", this->TotalTimeLevel);
        // [Game Analytics] Sending data about successful moves
        UGameAnalytics::AddDesignEventWithValue("MoveCount", this->CountMoves);

        // [Game Analytics] Sending a level start event
        const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
        UGameAnalytics::AddProgressionEventWithOneAndScore(EGAProgressionStatus::complete, LevelName, this->CountCoin);

        // Saving Data
        if (this->GameInstance->GetCountCoinsOnLevel() == 0 && this->CountCoin != 0) this->GameInstance->OpenNextLevel();
        this->SavingData();
    }

    if (NewState == EGameState::GameOver)
    {
        // [Game Analytics] Sending a level start event
        const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
        const FIntPoint PointPos = this->CameraPawn->GetAIPirate()->GetPointPosition();
        UGameAnalytics::AddProgressionEventWithOneTwoAndThree(
            EGAProgressionStatus::fail, LevelName, "pirate_pos", FString::FromInt(PointPos.X) + "_" + FString::FromInt(PointPos.X));
    }

    this->GameState = NewState;
    this->OnGameStateChanged.Broadcast(NewState);
}

void AGamePlayMode::OnChangeGameStateTimer(EGameState State, float Time)
{
    const float TempTime = (Time == 0.f) ? this->RateTime : Time;
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, "OnChangeGameState", State);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TempTime, false);
}

void AGamePlayMode::SpawnEffect(UNiagaraSystem* Effect, FVector Location)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Location);
}

void AGamePlayMode::SpawnEffectTimer(UNiagaraSystem* Effect, FVector Location, float RateTimeEffect)
{
    if (RateTimeEffect == 0.0f)
    {
        UE_LOG(LogGamePlayMode, Warning, TEXT("Call function SpawnEffectTimer with rate time: 0.0f | Effect: %s"), *Effect->GetName());
        this->SpawnEffect(Effect, Location);
        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &AGamePlayMode::SpawnEffect, Effect, Location);
    GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTimeEffect, false);
}

void AGamePlayMode::StartPlay()
{
    Super::StartPlay();

    this->GridGeneratorPlatform =
        Cast<AGridGeneratorActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridGeneratorActor::StaticClass()));
    checkf(this->GridGeneratorPlatform, TEXT("Grid is nullptr"));

    this->CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
    checkf(this->CameraPawn, TEXT("Camera pawn is nullptr"));

    this->GameInstance = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
    checkf(this->GameInstance, TEXT("Game Instance is nullptr"));

    // Setting the timer to preload the level
    FTimerHandle TimerSpawnPlatform;
    FTimerHandle TimerSpawnWall;
    FTimerHandle TimerSpawnPirate;
    FTimerHandle TimerSpawnSkeletonRunner;
    FTimerHandle TimerSpawnGold;

    GetWorldTimerManager().SetTimer(TimerSpawnPlatform, this, &AGamePlayMode::SpawnPlatform, this->RateTimeLoading * 1.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnWall, this, &AGamePlayMode::SpawnWall, this->RateTimeLoading * 2.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnPirate, this, &AGamePlayMode::SpawnPirate, this->RateTimeLoading * 3.f, false);
    GetWorldTimerManager().SetTimer(
        TimerSpawnSkeletonRunner, this, &AGamePlayMode::SpawnSkeletonRunner, this->RateTimeLoading * 4.f, false);
    GetWorldTimerManager().SetTimer(TimerSpawnGold, this, &AGamePlayMode::SpawnGold, this->RateTimeLoading * 5.f, false);

    // Filling in the class array ASkeletonCannonPawn
    BaseUtils::FillArrayActorOfClass<ASkeletonCannonPawn>(GetWorld(), ASkeletonCannonPawn::StaticClass(), this->ArraySkeletonCannon);

    // Setting the preset to maximum quality
    this->GameSettings = UGameUserSettings::GetGameUserSettings();
    GameSettings->SetOverallScalabilityLevel(2);
    GameSettings->ApplySettings(true);
    this->FPSOnSecondDelegate.AddUObject(this, &AGamePlayMode::SetupPresetSettings);

    // Triggering States
    this->OnChangeGameState(EGameState::Loading);
    this->OnChangeGameStateTimer(EGameState::StartInfo, this->RateTimeLoading * 6.f);

    // [Game Analytics] Sending a level start event
    const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameAnalytics::AddProgressionEventWithOne(EGAProgressionStatus::start, LevelName);

#if UE_BUILD_SHIPPING || UE_BUILD_DEVELOPMENT
    // Change screen mode settings
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (UserSettings->GetFullscreenMode() != EWindowMode::Fullscreen)
    {
        UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
        UserSettings->ApplySettings(false);
    }
#endif

    // Play bg music
    UGameplayStatics::PlaySound2D(GetWorld(), this->GameBGSound);
    UGameplayStatics::PlaySound2D(GetWorld(), this->WaterSound);
}

void AGamePlayMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Add total time
    this->TotalTimeLevel += DeltaSeconds;

#if !UE_BUILD_SHIPPING
    // Calculation of FPS for each frame
    this->DeltaTimeOneSec += DeltaSeconds;
    this->CountFPS++;
    if (this->DeltaTimeOneSec >= 1.0f)
    {
        this->FPSOnSecondDelegate.Broadcast(this->CountFPS);
        this->DeltaTimeOneSec = 0.0f;
        this->CountFPS = 0;
    }
#endif

    if (!this->GameInstance->IsActiveAds())
    {
        this->GameInstance->ChangeCountDownAd(this->GameInstance->GetCountDownAd() - DeltaSeconds);
        if (this->GameInstance->GetCountDownAd() <= 0.0f)
        {
            this->GameInstance->ChangeActiveAds(true);
            this->GameInstance->ChangeCountDownAd(this->GameInstance->GetDefaultCountDownAd());
        }
    }
}

void AGamePlayMode::SavingData() const
{
    int32 GoldOnSave = this->GameInstance->GetCountCoinsOnLevel();
    if (this->CountCoin > GoldOnSave) GoldOnSave = this->CountCoin;
    this->GameInstance->SetupNewDataLevel(GoldOnSave, BaseUtils::GetLevelStateFromCoin(GoldOnSave));
    this->GameInstance->SaveGameToSlot();
}

void AGamePlayMode::StopAllSkeletonRunner()
{
    TArray<ASkeletonRunnerPawn*> TempArraySkeletonRunners = this->GridGeneratorPlatform->GetArraySkeletonRunners();
    for (const auto TempRunner : TempArraySkeletonRunners)
    {
        TempRunner->ChangeStateBrain(EStateBrain::None);
        TempRunner->StopMovement();
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("All skeleton runners is stop movement"));
}

void AGamePlayMode::StopAllSkeletonCannon()
{
    for (const auto TempCannon : this->ArraySkeletonCannon)
    {
        TempCannon->StopFireCannon();
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("All skeleton cannon is stop shoot"));
}

void AGamePlayMode::StartAllSkeletonRunner()
{
    TArray<ASkeletonRunnerPawn*> TempArraySkeletonRunners = this->GridGeneratorPlatform->GetArraySkeletonRunners();
    for (const auto TempRunner : TempArraySkeletonRunners)
    {
        TempRunner->ChangeStateBrain(EStateBrain::Idle);
        TempRunner->GoToCurrentPoint();
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("All skeleton runners is start movement"));
}

void AGamePlayMode::StartAllSkeletonCannon()
{
    for (const auto TempCannon : this->ArraySkeletonCannon)
    {
        TempCannon->StartFireCannon();
    }
    UE_LOG(LogGamePlayMode, Display, TEXT("All skeleton cannon is start shoot"));
}

void AGamePlayMode::SetupPresetSettings(int32 FPS)
{
    const int32 NumberQuality = this->GameSettings->GetOverallScalabilityLevel();
    this->MaxCountFPS = FPS;

    UE_LOG(LogGamePlayMode, Display, TEXT("Setup Preset Settings | Count FPS: %i | Current number quality: %i "), FPS, NumberQuality);

    if (NumberQuality == 0 && FPS < this->LowerEdgeFPS)
        UGameAnalytics::AddErrorEvent(EGAErrorSeverity::warning, FString::Printf(TEXT("NumberQuality: 0 | FPS: %i"), FPS));

    if ((NumberQuality == 0 && FPS < this->LowerEdgeFPS) || (NumberQuality == 4 && FPS > this->UpperEdgeFPS)) return;

    if (FPS > this->UpperEdgeFPS && NumberQuality != 4)
    {
        GameSettings->SetOverallScalabilityLevel(NumberQuality + 1);
        GameSettings->ApplySettings(true);
    }
    if (FPS < this->LowerEdgeFPS && NumberQuality != 0)
    {
        GameSettings->SetOverallScalabilityLevel(NumberQuality - 1);
        GameSettings->ApplySettings(true);
    }
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
    // Get Last point and position
    APiratePawn* TempPirate = this->CameraPawn->GetAIPirate();
    const FIntPoint LastPoint = TempPirate->GetPreviousPoint();
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
    TempPirate->SetActorRotation(FRotator(0.0f));
    this->CameraPawn->SetupDirectionPlayer(EDirectionPlayer::Right);
    TempPirate->ChangeStateBrain(EStateBrain::Idle);
    TempPirate->SetNewPoint(LastPoint);
    TempPirate->GetCollision()->SetCollisionProfileName("PirateCollision");

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->TeleportOutEffect, ResetToLocation);

    this->OnChangeGameState(EGameState::GameProgress);
}

void AGamePlayMode::IncreaseCountCoin()
{
    this->CountCoin++;
    this->OnCoinIncrease.Broadcast(this->CountCoin);
}
