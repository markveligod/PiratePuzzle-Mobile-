// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/UI/WelcomeMenuUserWidget.h"

#include "PPGameInstance.h"
#include "Components/Image.h"
#include "Menu/GameMenuMode.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWelcomeMenuUserWidget, All, All);

void UWelcomeMenuUserWidget::ChangeActiveWidget(bool bNewState)
{
    Super::ChangeActiveWidget(bNewState);
    if (bNewState)
    {
        if (GetPPGameInstance()->IsFirstRunGame()) PlayAnimationTimer(this->WelcomeAnimation, this->StartAnim->GetEndTime());
        GetGameMenuMode()->ChangeMenuStateTimer(EMenuState::MenuBase, this->WelcomeAnimation->GetEndTime());
        UE_LOG(LogWelcomeMenuUserWidget, Display, TEXT("State Widget: %s | Play Animation: %s | Rate Time Animation: %f"),
            *BaseUtils::GetStringFromBool(bNewState), *this->WelcomeAnimation->GetName(), this->WelcomeAnimation->GetEndTime());
    }
}

void UWelcomeMenuUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UE_LOG(LogWelcomeMenuUserWidget, Display, TEXT("Native On Initialized"));
}
