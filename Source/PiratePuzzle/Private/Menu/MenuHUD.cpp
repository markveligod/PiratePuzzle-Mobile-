// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/MenuHUD.h"
#include "Menu/UI/MenuUserWidget.h"
#include "Menu/GameMenuMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogMenuHUD, All, All);

void AMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    this->GameMode = Cast<AGameMenuMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game Menu mode is nullptr"));
    this->GameMode->OnMenuStateChanged.AddUObject(this, &AMenuHUD::OnChangeMenuState);

    checkf(this->WelcomeWidgetClass, TEXT("Welcome widget class is nullptr"));
    checkf(this->MenuWidgetClass, TEXT("Menu widget class is nullptr"));

    // Create and add widget
    this->GameWidgets.Add(EMenuState::Welcome, CreateWidget<UMenuUserWidget>(GetWorld(), this->WelcomeWidgetClass));
    this->GameWidgets.Add(EMenuState::MenuBase, CreateWidget<UMenuUserWidget>(GetWorld(), this->MenuWidgetClass));

    // Set Hidden and add to viewport
    for (const auto Widget : this->GameWidgets)
    {
        const auto ValueWidget = Widget.Value;
        ValueWidget->ChangeActiveWidget(false);
        ValueWidget->AddToViewport();
        ValueWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMenuHUD::OnChangeMenuState(EMenuState NewState)
{
    if (!this->GameWidgets.Contains(NewState))
    {
        UE_LOG(LogMenuHUD, Error, TEXT("Map game widgets doesn't contain %s"), *UEnum::GetValueAsString(NewState));
        return;
    }

    if (this->GameWidgets[NewState] == this->WidgetOnVisible)
    {
        UE_LOG(LogMenuHUD, Error, TEXT("Widget On Visible: %s equal New State: %s"), *this->WidgetOnVisible->GetName(),
            *UEnum::GetValueAsString(NewState));
        return;
    }

    // Play end animation if widget != nullptr
    if (this->WidgetOnVisible)
    {
        this->WidgetOnVisible->ChangeActiveWidget(false);
        this->WidgetOnVisible->ShowAnim(this->WidgetOnVisible->EndAnim);

        if (this->WidgetOnVisible->EndAnim->GetEndTime() == 0.0f)
        {
            this->ChangeOnNewWidget(NewState);
        }
        else
        {
            FTimerDelegate TempDelegate;
            TempDelegate.BindUFunction(this, "ChangeOnNewWidget", NewState);
            GetWorld()->GetTimerManager().SetTimer(
                this->TimerHandleCollapse, TempDelegate, this->WidgetOnVisible->EndAnim->GetEndTime(), false);
        }
    }
    else
        this->ChangeOnNewWidget(NewState);
}

void AMenuHUD::ChangeOnNewWidget(EMenuState NewState)
{
    if (GetWorld()->GetTimerManager().TimerExists(this->TimerHandleCollapse))
    {
        UE_LOG(LogMenuHUD, Display, TEXT("Name: %s | Struct TimerHandleCollapse exist!"), *GetName())
        GetWorld()->GetTimerManager().ClearTimer(this->TimerHandleCollapse);
    }
    if (this->WidgetOnVisible) this->WidgetOnVisible->SetVisibility(ESlateVisibility::Hidden);

    this->WidgetOnVisible = this->GameWidgets[NewState];
    this->WidgetOnVisible->SetVisibility(ESlateVisibility::Visible);
    this->WidgetOnVisible->ShowAnim(this->WidgetOnVisible->StartAnim);
    this->WidgetOnVisible->ChangeActiveWidgetTimer(true, this->WidgetOnVisible->StartAnim->GetEndTime());
}
