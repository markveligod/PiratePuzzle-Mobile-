// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/GameHUD.h"

#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/AI/SkeletonRunner/SkeletonRunnerCharacter.h"
#include "Game/Camera/CameraPawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Game/HUD/UI/BaseUserWidget.h"

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

    checkf(this->GameWidgetClass, TEXT("Game widget class is nullptr"));

    auto Widget = CreateWidget<UBaseUserWidget>(GetWorld(), this->GameWidgetClass);
    if (Widget)
    {
        Widget->AddToViewport();
    }

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
}
