// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/GameHUD.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerPawn.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

void AGameHUD::DrawHUD()
{
    Super::DrawHUD();

#if !UE_BUILD_SHIPPING
    ACameraPawn* CameraPawn = Cast<ACameraPawn>(GetOwningPawn());

    if (this->GameMode && EnableDebugHUD)
    {
        AddText(TEXT("----------Game Mode----------"), FText::FromString(""));
        AddText(TEXT("Current run level:"), FText::FromString(UGameplayStatics::GetCurrentLevelName(GetWorld())));
        AddText(TEXT("Game play state:"), FText::FromString(UEnum::GetValueAsString(this->GameMode->GetGameState())));
        AddInt(TEXT("Current coins pickup:"), this->GameMode->GetCountCoin());
        AddInt(TEXT("Max count FPS:"), this->GameMode->MaxCountFPS);
        AddInt(TEXT("Current Render Qualities:"), this->GameMode->GameSettings->GetOverallScalabilityLevel());
    }

    if (CameraPawn && CameraPawn->AIPirate && EnableDebugHUD)
    {
        AddText(TEXT("----------Pirate Data----------"), FText::FromString(""));
        AddText(TEXT("Current pirate state:"), FText::FromString(UEnum::GetValueAsString(CameraPawn->AIPirate->GetStateBrain())));
        AddText(TEXT("Current Point Position:"), FText::FromString(CameraPawn->AIPirate->GetPointPosition().ToString()));
        AddText(TEXT("Current Direction:"), FText::FromString(UEnum::GetValueAsString(CameraPawn->DirectionPlayer)));
        AddText(TEXT("Start Touch on screen:"), FText::FromString(CameraPawn->StartTouch.ToString()));
        AddText(TEXT("End Touch on screen:"), FText::FromString(CameraPawn->EndTouch.ToString()));
    }

    if (this->GameMode && EnableDebugHUD)
    {
        auto ArrayPointerSkeletons = this->GameMode->GetGridGenerator()->GetArraySkeletonRunners();
        for (int32 i = 0; i < ArrayPointerSkeletons.Num(); i++)
        {
            AddText(TEXT("----------Skeleton Runner----------"), FText::FromString("#" + FString::FromInt(i)));
            AddText(TEXT("Current Skeleton state:"), FText::FromString(UEnum::GetValueAsString(ArrayPointerSkeletons[i]->GetStateBrain())));
            AddText(TEXT("Current Position:"), FText::FromString(ArrayPointerSkeletons[i]->GetPositionSkeleton().ToString()));
        }
    }
#endif
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameMode->OnGameStateChanged.AddUObject(this, &AGameHUD::OnChangeGameState);

    checkf(this->StartInfoWidgetClass, TEXT("Start Info widget class is nullptr"));
    checkf(this->GameWidgetClass, TEXT("Game widget class is nullptr"));
    checkf(this->PauseWidgetClass, TEXT("Pause widget class is nullptr"));
    checkf(this->GameWinWidgetClass, TEXT("Game Win widget class is nullptr"));
    checkf(this->GameOverWidgetClass, TEXT("Game Over widget class is nullptr"));
    checkf(this->LoadingWidgetClass, TEXT("Loading widget class is nullptr"));

    // Create and add widget
    this->GameWidgets.Add(EGameState::StartInfo, CreateWidget<UBaseUserWidget>(GetWorld(), this->StartInfoWidgetClass));
    this->GameWidgets.Add(EGameState::GameProgress, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameWidgetClass));
    this->GameWidgets.Add(EGameState::Pause, CreateWidget<UBaseUserWidget>(GetWorld(), this->PauseWidgetClass));
    this->GameWidgets.Add(EGameState::GameWin, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameWinWidgetClass));
    this->GameWidgets.Add(EGameState::GameOver, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameOverWidgetClass));
    this->GameWidgets.Add(EGameState::Loading, CreateWidget<UBaseUserWidget>(GetWorld(), this->LoadingWidgetClass));
    this->GameWidgets.Add(EGameState::AdsSkill, CreateWidget<UBaseUserWidget>(GetWorld(), this->AdsCountWidgetClass));

    // Set Hidden and add to viewport
    for (const auto Widget : this->GameWidgets)
    {
        const auto ValueWidget = Widget.Value;
        ValueWidget->AddToViewport();
        ValueWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGameHUD::OnChangeGameState(EGameState NewState)
{

    if (!this->GameWidgets.Contains(NewState))
    {
        UE_LOG(LogGameHUD, Error, TEXT("Map game widgets doesn't contain %s"), *UEnum::GetValueAsString(NewState));
        return;
    }

    if (this->GameWidgets[NewState] == this->WidgetOnVisible)
    {
        UE_LOG(LogGameHUD, Error, TEXT("Widget On Visible: %s equal New State: %s"), *this->WidgetOnVisible->GetName(),
            *UEnum::GetValueAsString(NewState));
        return;
    }

    // Play end animation if widget != nullptr
    if (this->WidgetOnVisible)
    {
        this->WidgetOnVisible->DisableButtonActive();
        this->WidgetOnVisible->ShowAnim(this->WidgetOnVisible->GetEndAnim());
        FTimerDelegate TempDelegate;
        TempDelegate.BindUFunction(this, "ChangeOnNewWidget", NewState);
        GetWorld()->GetTimerManager().SetTimer(this->TimerHandleCollapse, TempDelegate, this->WidgetOnVisible->GetRateTimeEndAnim(), false);
    }
    else
        this->ChangeOnNewWidget(NewState);
}

void AGameHUD::ChangeOnNewWidget(EGameState NewState)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleCollapse))
    {
        UE_LOG(LogGameHUD, Display, TEXT("Name: %s | Struct TimerHandleCollapse exist!"), *GetName())
        GetWorld()->GetTimerManager().ClearTimer(this->TimerHandleCollapse);
    }
    if (this->WidgetOnVisible) this->WidgetOnVisible->SetVisibility(ESlateVisibility::Hidden);

    this->WidgetOnVisible = this->GameWidgets[NewState];
    this->WidgetOnVisible->SetVisibility(ESlateVisibility::Visible);
    this->WidgetOnVisible->EnableButtonActive();
    this->WidgetOnVisible->ShowAnim(this->WidgetOnVisible->GetStartAnim());
}
