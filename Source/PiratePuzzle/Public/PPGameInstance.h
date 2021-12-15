// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/MenuDataTypes.h"
#include "PPGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCountMoveBackChangedSignature, int32);

class UPPSaveGame;
/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API UPPGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Delegate for signing to change the counter
    FCountMoveBackChangedSignature CountMoveBackChanged;

    // Getting Menu level name
    FName GetNameMenuLevel() const { return (this->MenuLevel); }

    // Getting All map levels
    const TMap<int32, FInfoLevel>& GetAllLevels() const { return (this->MapGameLevels); }

    // Getting All map StartInfo
    const TMap<FName, FText>& GetAllStartInfo() const { return (this->MapStartInfo); }

    // Is first run game ?
    bool IsFirstRunGame() const { return (this->bFirstGameRun); }

    // Setting state first run game
    void SetStateFirstRunGame(bool NewState) { this->bFirstGameRun = NewState; }

    // Call Save current save
    void SaveGameToSlot();

    // Setting new data for the current level launch
    void SetupNewDataLevel(int32 CountCoin, ELevelState NewState = ELevelState::Lock);

    // Setting run Level
    void SetRunLevel(const int32 NewNumLevel);

    // Getting run level index
    int32 GetRunLevel() const { return (this->NumLevel); }

    // Getting the number of coins in the saved level
    int32 GetCountCoinsOnLevel() const;

    // Getting level data by index
    FInfoLevel GetInfoLevel(int32 Index) const;

    // Open the next level interval
    void OpenNextLevel();

    // Getting starting value music and sound volume
    float GetDefaultMusicVolume() const { return (this->DefaultValueMusicVolume); }
    float GetDefaultSoundVolume() const { return (this->DefaultValueSoundVolume); }

    // Getting current pointer of music and sound class
    USoundClass* GetMusicClass() const { return (this->MusicClass); }
    USoundClass* GetSoundClass() const { return (this->SoundClass); }

    // Setup Count move back
    UFUNCTION(BlueprintCallable, Category = "APiratePawn")
    void SetupCountMoveBack(int32 NewCount);
    // Setup Count move back on Timer
    UFUNCTION(BlueprintCallable, Category = "APiratePawn")
    void SetupCountMoveBackTimer(int32 NewCount, float RateTime = 0.0f);

    // Getting Count move back
    UFUNCTION(BlueprintCallable, Category = "APiratePawn", meta = (ToolTip = "Getting Count move back"))
    int32 GetCountMoveBack() const { return (this->CountMoveBack); }

    // Getting default value count move back
    int32 GetDefaultCountMoveBack() const { return (this->DefaultCountAdd); }

    // Is active ads ?
    bool IsActiveAds() const { return (this->bActiveAds); }
    // Change state Active ads
    void ChangeActiveAds(const bool NewState) { this->bActiveAds = NewState; }
    // Change Count down ad
    void ChangeCountDownAd(const float NewValue) { this->CountDownAdYelps = NewValue; }
    // Getting count down ad value
    float GetCountDownAd() const { return (this->CountDownAdYelps); }
    // Getting Default count
    float GetDefaultCountDownAd() const { return (this->CountDownAd); }

    // Change language
    void ChangeLanguage(const FString& NewLang);

    // exec to open all levels
    UFUNCTION(Exec)
    void OpenAllLevels();

protected:
    virtual void OnStart() override;
    virtual void Init() override;

private:
    // Param for control ads
    bool bActiveAds = true;
    // Default values for the countdown time for ad activation (sec)
    UPROPERTY(EditDefaultsOnly, Category = "Settings Ads", meta = (ClampMin = "0.0"))
    float CountDownAd = 180.0f;
    // The interval for counting
    float CountDownAdYelps = 0.0f;

    // How much do I need to install a counter for viewing ads
    UPROPERTY(EditDefaultsOnly, Category = "Settings Ads", meta = (ClampMin = "1", ClampMax = "10"))
    int32 DefaultCountAdd = 3;
    /*
     * parameters Audio storage and settings
     */
    // Starting value music volume
    UPROPERTY(EditDefaultsOnly, Category = "Settings Volume",
        meta = (AllowPivateAccess = true, ClampMin = 0.0, ClampMax = 1.0, ToolTip = "Starting value music volume"))
    float DefaultValueMusicVolume = 0.5f;
    // Starting value sound volume
    UPROPERTY(EditDefaultsOnly, Category = "Settings Volume",
        meta = (AllowPivateAccess = true, ClampMin = 0.0, ClampMax = 1.0, ToolTip = "Starting value sound volume"))
    float DefaultValueSoundVolume = 0.5f;
    // Music class
    UPROPERTY(EditDefaultsOnly, Category = "Settings Volume", meta = (AllowPivateAccess = true, ToolTip = "Music class"))
    USoundClass* MusicClass;
    // Sound class
    UPROPERTY(EditDefaultsOnly, Category = "Settings Volume", meta = (AllowPivateAccess = true, ToolTip = "Sound class"))
    USoundClass* SoundClass;

    // Name of the level menu
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Levels",
        meta = (AllowPrivateAccess = true, ToolTip = "Name of the level menu"))
    FName MenuLevel = NAME_None;

    // Map game levels which contains the numbering of the key level and its name in the value
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Levels",
        meta = (AllowPrivateAccess = true,
            ToolTip = "Map game levels which contains the numbering of the key level and its name in the value"))
    TMap<int32, FInfoLevel> MapGameLevels;

    // The interval between the opening of the level
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Levels",
        meta = (AllowPrivateAccess = true, ToolTip = "The interval between the opening of the level."))
    int32 IntervalCountLevel = 1;

    // The data map for each level corresponds to the text that will be displayed at the start of the level in the WBP_StartInfo widget.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Levels",
        meta = (AllowPrivateAccess = true,
            ToolTip = "The data map for each level corresponds to the text that will be displayed at the start of the level."))
    TMap<FName, FText> MapStartInfo;

    // Name of the slot to save in the class.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Save",
        meta = (AllowPrivateAccess = true, ToolTip = "Name of the slot to save in the class."))
    FName SlotSave = "SaveGame";
    // The user's index for saving
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Save",
        meta = (AllowPrivateAccess = true, ToolTip = "The user's index for saving."))
    int32 UserIndex = 0;

    // How many moves back can he make in a session?
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings Skill",
        meta = (AllowPrivateAccess = "true", ToolTip = "How many moves back can he make in a session?", ClampMin = "1", ClampMax = "10"))
    int32 CountMoveBack = 3;

    // The current level is running
    int32 NumLevel = 0;

    // Save class
    UPROPERTY()
    UPPSaveGame* SaveGame;

    // State first game run
    bool bFirstGameRun = true;

    // Function for logging the level map
    void PrintLogMap() const;
};
