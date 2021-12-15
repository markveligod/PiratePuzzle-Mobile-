// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Menu/UI/Components/MenuButton.h"
#include "Components/ButtonSlot.h"

TSharedRef<SWidget> UMenuButton::RebuildWidget()
{
    MyButton = SNew(SButton)
                   .OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, MenuSlateHandleClicked))  // !!!
                   .OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
                   .OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
                   .OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
                   .OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
                   .ButtonStyle(&WidgetStyle)
                   .ClickMethod(ClickMethod)
                   .TouchMethod(TouchMethod)
                   .PressMethod(PressMethod)
                   .IsFocusable(IsFocusable);

    if (GetChildrenCount() > 0)
    {
        Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
    }

    return MyButton.ToSharedRef();
}

FReply UMenuButton::MenuSlateHandleClicked()
{
    OnClicked.Broadcast();
    if (this->IndexLevel != 0) this->OnMenuClick.Broadcast(this->IndexLevel, this->AnimToPlay);
    return FReply::Handled();
}
