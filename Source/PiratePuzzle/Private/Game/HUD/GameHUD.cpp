// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/GameHUD.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Game/Camera/CameraPawn.h"
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
}
