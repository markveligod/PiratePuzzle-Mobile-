// Pirate Puzzle. Contact: markveligod@gmail.com

#include "PPGameInstance.h"
#include "EasyAdsLibrary.h"
#include "PPSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "UtilsLib/BaseUtils.h"
#include "GameAnalytics.h"
#include "PlayVideoProxy.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "GameFramework/GameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPGameInstance, All, All);

void UPPGameInstance::SaveGameToSlot()
{
    // Set data on the save game object.
    this->SaveGame = Cast<UPPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPPSaveGame::StaticClass()));
    UE_LOG(LogPPGameInstance, Display, TEXT("Create Status: %s"), *BaseUtils::GetStringFromBool(this->SaveGame != nullptr));
    if (!this->SaveGame)
    {
        UE_LOG(LogPPGameInstance, Error, TEXT("Variable SaveGame is nullptr | Load to SaveGame - FAIL"))
        return;
    }

    this->SaveGame->SaveSlotName = this->SlotSave.ToString();
    this->SaveGame->SaveCountMoveBack = this->CountMoveBack;
    this->SaveGame->UserIndex = UserIndex;
    this->SaveGame->MapSavedLevels = this->MapGameLevels;
    this->SaveGame->MusicVolume = this->MusicClass->Properties.Volume;
    this->SaveGame->SoundVolume = this->SoundClass->Properties.Volume;

    // Save the data immediately.
    const bool Status = UGameplayStatics::SaveGameToSlot(this->SaveGame, this->SlotSave.ToString(), this->UserIndex);
    UE_LOG(LogPPGameInstance, Display, TEXT("Name slot: %s | User index: #%i | Save Status: %s"), *this->SaveGame->SaveSlotName,
        this->SaveGame->UserIndex, *BaseUtils::GetStringFromBool(Status));
}

void UPPGameInstance::SetupNewDataLevel(int32 CountCoin, ELevelState NewState)
{
    if (this->MapGameLevels.Contains(this->NumLevel))
    {
        this->MapGameLevels[this->NumLevel].CountCoin = CountCoin;
        this->MapGameLevels[this->NumLevel].LevelState = NewState;
    }
    else
    {
        UE_LOG(LogPPGameInstance, Error, TEXT("The data is not saved because the level data doesn't exist | Current Index level: %i"),
            this->NumLevel);
    }
}

void UPPGameInstance::SetRunLevel(const int32 NewNumLevel)
{
    this->NumLevel = NewNumLevel;
    if (NewNumLevel % 10 == 0 && NewNumLevel != 0)
    {
        // Activate rewarded video
        UPlayVideoProxy* PlayVideoProxy = NewObject<UPlayVideoProxy>();
        if (UEasyAdsLibrary::IsRewardedVideoReady())
        {
            PlayVideoProxy->Activate();
            this->bActiveAds = false;
            UGameAnalytics::AddAdEvent(EGAAdAction::show, EGAAdType::rewardedvideo, "admob", "show_fivelevel");
        }
    }
}

int32 UPPGameInstance::GetCountCoinsOnLevel() const
{
    int32 RetNumCoin = 0;
    if (this->MapGameLevels.Contains(this->NumLevel))
    {
        RetNumCoin = this->MapGameLevels[this->NumLevel].CountCoin;
    }
    else
    {
        UE_LOG(LogPPGameInstance, Error, TEXT("The data is not saved because the level data doesn't exist | Current Index level: %i"),
            this->NumLevel);
    }
    return (RetNumCoin);
}

FInfoLevel UPPGameInstance::GetInfoLevel(const int32 Index) const
{
    if (this->MapGameLevels.Contains(Index)) return (this->MapGameLevels[Index]);
    return (FInfoLevel());
}

void UPPGameInstance::OpenNextLevel()
{
    for (int32 i = 1; i <= this->MapGameLevels.Num(); i++)
    {
        if (this->MapGameLevels[i].LevelState == ELevelState::Lock)
        {
            this->MapGameLevels[i].LevelState = ELevelState::ZeroGold;
            break;
        }
    }
}

void UPPGameInstance::SetupCountMoveBack(int32 NewCount)
{
    this->CountMoveBack = NewCount;
    this->CountMoveBackChanged.Broadcast(NewCount);
    UE_LOG(LogPPGameInstance, Display, TEXT("New count move back: %i | Broadcast"), NewCount);
    this->SaveGameToSlot();
}

void UPPGameInstance::SetupCountMoveBackTimer(int32 NewCount, float RateTime)
{
    if (RateTime == 0.0f)
    {
        UE_LOG(LogPPGameInstance, Warning, TEXT("Call function with Timer: 0.0f"));
        this->SetupCountMoveBack(NewCount);
        return;
    }

    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UPPGameInstance::SetupCountMoveBack, NewCount);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RateTime, false);
    UE_LOG(LogPPGameInstance, Display, TEXT("Change count move back: %i | on timer: %f"), NewCount, RateTime);
}

void UPPGameInstance::ChangeLanguage(const FString& NewLang)
{
    UKismetInternationalizationLibrary::SetCurrentLanguage(NewLang);
    UE_LOG(LogPPGameInstance, Display, TEXT("New lang: %s"), *NewLang);
}

void UPPGameInstance::OpenAllLevels()
{
    for (int32 i = 1; i <= this->MapGameLevels.Num(); i++)
    {
        if (this->MapGameLevels[i].LevelState == ELevelState::Lock)
        {
            this->MapGameLevels[i].LevelState = ELevelState::ZeroGold;
        }
    }
}

void UPPGameInstance::OnStart()
{
    Super::OnStart();

#if UE_BUILD_SHIPPING || UE_BUILD_DEVELOPMENT
    // Change screen mode settings
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (UserSettings->GetFullscreenMode() != EWindowMode::Fullscreen)
    {
        UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
        UserSettings->ApplySettings(false);
    }
#endif

    // check language
    const FString Lang = UKismetInternationalizationLibrary::GetCurrentCulture();
    this->ChangeLanguage((Lang == "ru" || Lang == "ru-RU" || Lang == "ru-UA" || Lang == "ru-BY") ? "ru" : "en");

    // Game Analytics start and init
    UGameAnalytics::startSession();
    UGameAnalytics::initialize("95017c2a9d3008d78463f1659f5d4a40", "c6372a2fea1e29098005bec4d85acd9ca6e1b24f");

    // Init count down ads
    this->bActiveAds = true;
    this->CountDownAdYelps = this->CountDownAd;

#if !UE_BUILD_SHIPPING
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FString("stat FPS"));
    UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), FString("stat UNIT"));
#endif
}

void UPPGameInstance::Init()
{
    Super::Init();
    if (UGameplayStatics::DoesSaveGameExist(this->SlotSave.ToString(), this->UserIndex))
    {
        // saving exists
        UE_LOG(LogPPGameInstance, Display, TEXT("Save game exist"));

        this->SaveGame = Cast<UPPSaveGame>(UGameplayStatics::LoadGameFromSlot(this->SlotSave.ToString(), this->UserIndex));
        if (!this->SaveGame)
        {
            UE_LOG(LogPPGameInstance, Error, TEXT("Variable SaveGame is nullptr | Load to SaveGame - FAIL"))
            return;
        }
        UE_LOG(
            LogPPGameInstance, Display, TEXT("Load Status: %s | User Index: %i"), *this->SaveGame->SaveSlotName, this->SaveGame->UserIndex);
        this->MapGameLevels = this->SaveGame->MapSavedLevels;
        this->CountMoveBack = this->SaveGame->SaveCountMoveBack;
        this->MusicClass->Properties.Volume = this->SaveGame->MusicVolume;
        this->SoundClass->Properties.Volume = this->SaveGame->SoundVolume;
    }
    else
    {
        // saving does not exist
        UE_LOG(LogPPGameInstance, Display, TEXT("Save game dont exist"));
        this->SaveGame = Cast<UPPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPPSaveGame::StaticClass()));
        if (!this->SaveGame)
        {
            UE_LOG(LogPPGameInstance, Error, TEXT("Variable SaveGame is nullptr | Load to SaveGame - FAIL"))
            return;
        }
        this->SaveGame->SaveSlotName = this->SlotSave.ToString();
        this->SaveGame->UserIndex = UserIndex;

        // Init volume
        this->MusicClass->Properties.Volume = this->DefaultValueMusicVolume;
        this->SoundClass->Properties.Volume = this->DefaultValueSoundVolume;
        this->SaveGame->MusicVolume = this->DefaultValueMusicVolume;
        this->SaveGame->SoundVolume = this->DefaultValueSoundVolume;

        // Init new map levels
        int32 CountNotPassLevel = 0;
        UE_LOG(LogPPGameInstance, Display, TEXT("Start init map game levels..."))
        for (int32 i = 1; i <= this->MapGameLevels.Num(); i++)
        {
            ELevelState TempState = ELevelState::Lock;
            if (CountNotPassLevel < this->IntervalCountLevel)
            {
                TempState = ELevelState::ZeroGold;
                CountNotPassLevel++;
            }
            this->MapGameLevels[i].LevelState = TempState;
        }
        UE_LOG(LogPPGameInstance, Display, TEXT("End init map game levels..."))

        // Save the data immediately.
        this->SaveGame->MapSavedLevels = this->MapGameLevels;
        const bool Status = UGameplayStatics::SaveGameToSlot(this->SaveGame, this->SlotSave.ToString(), this->UserIndex);
        UE_LOG(LogPPGameInstance, Display, TEXT("Name slot: %s | User index: #%i | Save Status: %s"), *this->SaveGame->SaveSlotName,
            this->SaveGame->UserIndex, *BaseUtils::GetStringFromBool(Status));
    }
    this->PrintLogMap();
}

void UPPGameInstance::PrintLogMap() const
{
    UE_LOG(LogPPGameInstance, Display, TEXT("...|Print log map game levels|..."))
    for (auto CurrentLevel : this->MapGameLevels)
    {
        UE_LOG(LogPPGameInstance, Display, TEXT("Level index: #%i | Level name: %s | Level state: %s"), CurrentLevel.Key,  //
            *CurrentLevel.Value.NameLevel.ToString(),                                                                      //
            *UEnum::GetValueAsString(CurrentLevel.Value.LevelState));                                                      //
    }
}
