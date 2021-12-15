// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/StartInfoUserWidget.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInternationalizationLibrary.h"

void UStartInfoUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->CloseButton->OnClicked.AddDynamic(this, &UStartInfoUserWidget::CloseWidget);
    this->LangButton->OnClicked.AddDynamic(this, &UStartInfoUserWidget::SwipeLang);

    const auto AllMapStartInfo = GetPPGameInstance()->GetAllStartInfo();
    const FText InfoText = AllMapStartInfo[FName(UGameplayStatics::GetCurrentLevelName(GetWorld()))];
    this->InfoTextBlock->SetText(InfoText);
}

void UStartInfoUserWidget::CloseWidget()
{
    if (!GetStateActiveButton()) return;
    ShowAnim(this->CloseButtonAnim);
    GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameProgress, this->DelayCloseButton);
    DisableButtonActive();
}

void UStartInfoUserWidget::SwipeLang()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    ButtonActiveTimer(true, this->LangButtonAnim->GetEndTime());

    ShowAnim(this->LangButtonAnim);
    const FString Lang = UKismetInternationalizationLibrary::GetCurrentCulture();
    GetPPGameInstance()->ChangeLanguage((Lang == "en") ? "ru" : "en");
}
