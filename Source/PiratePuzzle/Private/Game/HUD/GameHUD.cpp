// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/GameHUD.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/UI/BaseUserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

void AGameHUD::DrawHUD()
{
    Super::DrawHUD();
    ACameraPawn* CameraPawn = Cast<ACameraPawn>(GetOwningPawn());

    if (CameraPawn && EnableDebugHUD)
    {
        AddText(TEXT("----------Pirate Data----------"), FText::FromString(""));
        AddText(TEXT("Current pirate state:"), FText::FromString(UEnum::GetValueAsString(CameraPawn->AIPlayer->GetStateAI())));
        AddText(TEXT("Current Position:"), FText::FromString(CameraPawn->AIPlayer->GetPosPlayer().ToString()));
    }

    if (this->GameMode && EnableDebugHUD)
    {
        auto ArrayPointerSkeletons = this->GameMode->GetGridGenerator()->GetArraySkeletonPointers();
        for (int32 i = 0; i < ArrayPointerSkeletons.Num(); i++)
        {
            AddText(TEXT("----------Skeleton Runner----------"), FText::FromString("#" + FString::FromInt(i)));
            AddText(TEXT("Current Skeleton state:"),
                FText::FromString(UEnum::GetValueAsString(ArrayPointerSkeletons[i]->GetStateAISkeletonRunner())));
            AddText(TEXT("Current Position:"), FText::FromString(ArrayPointerSkeletons[i]->GetPositionSkeleton().ToString()));
        }
    }
}

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameMode->OnGameStateChanged.AddUObject(this, &AGameHUD::OnChangeGameState);

    checkf(this->StartInfoWidgetClass, TEXT("Start Info widget class is nullptr"));
    checkf(this->GameWidgetClass, TEXT("Game widget class is nullptr"));

    // Create and add widget
    this->GameWidgets.Add(EGameState::StartInfo, CreateWidget<UBaseUserWidget>(GetWorld(), this->StartInfoWidgetClass));
    this->GameWidgets.Add(EGameState::GameProgress, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameWidgetClass));

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
