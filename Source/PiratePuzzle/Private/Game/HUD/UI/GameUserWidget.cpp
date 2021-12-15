// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameUserWidget.h"
#include "GameAnalytics.h"
#include "NiagaraFunctionLibrary.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Game/GamePlayMode.h"
#include "Game/Camera/CameraPawn.h"
#include "UtilsLib/BaseUtils.h"
#include "Components/Image.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "UtilsLib/LoaderUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameUserWidget, All, All);

void UGameUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->CameraButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedSwapCamera);
    this->BackButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedBackPos);
    this->PauseButton->OnClicked.AddDynamic(this, &UGameUserWidget::OnClickedPauseState);

    GetGamePlayMode()->OnCoinIncrease.AddUObject(this, &UGameUserWidget::ChangeCountCoin);

    this->UpdateTextCount(GetPPGameInstance()->GetCountMoveBack());
    GetPPGameInstance()->CountMoveBackChanged.AddUObject(this, &UGameUserWidget::UpdateTextCount);

    UE_LOG(LogGameUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameUserWidget::OnClickedSwapCamera()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    ButtonActiveTimer(true, DelayCloseButton);

    PlayAnimation(this->CameraButtonAnim);
    GetGamePlayMode()->GetCameraPawn()->StartSwapCamera();
}

void UGameUserWidget::OnClickedBackPos()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    ButtonActiveTimer(true, this->BackButtonAnim->GetEndTime() + this->RateTimeTeleportIn);

    if (GetPPGameInstance()->GetCountMoveBack() == 0)
    {
        UE_LOG(LogGameUserWidget, Warning, TEXT("Call widget with ads"))
        GetGamePlayMode()->OnChangeGameState(EGameState::AdsSkill);
        return;
    }

    APiratePawn* TempPirate = GetGamePlayMode()->GetCameraPawn()->GetAIPirate();
    if (TempPirate->GetStateBrain() != EStateBrain::Idle) return;

    const FIntPoint LastPoint = GetGamePlayMode()->GetCameraPawn()->GetAIPirate()->GetPreviousPoint();
    if (LastPoint == FIntPoint(-1, -1)) return;

    PlayAnimation(this->BackButtonAnim);

    auto TempMap = GetGamePlayMode()->GetGridGenerator()->GetMapPlatformOnGrid();
    FVector NewPos = BaseUtils::GetVectorPositionPlatform(LastPoint, TempMap);
    NewPos.Z = TempPirate->GetActorLocation().Z;

    // Effect teleport IN and play sound
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->TeleportInEffect, TempPirate->GetActorLocation());
    PlaySoundCueTimer(this->TeleportStartSound, this->RateTimeTeleportIn - 0.2f);

    // Play Anim Jump
    TempPirate->PlayAnimMontage(this->AnimJumpPirate);
    float StartTime;
    float EndTime;
    this->AnimJumpPirate->GetSectionStartAndEndTime(0, StartTime, EndTime);
    TempPirate->PlayAnimMontageTimer(this->AnimFlyPirate, EndTime);

    // Setting All pawn
    TempPirate->ChangeStateBrain(EStateBrain::None);
    GetGamePlayMode()->StopAllSkeletonCannon();
    GetGamePlayMode()->StopAllSkeletonRunner();

    // Setup timer for teleport
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &UGameUserWidget::TeleportObject, TempPirate, NewPos, LastPoint);
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, this->RateTimeTeleportIn, false);

    // Change count move back
    GetPPGameInstance()->SetupCountMoveBack(GetPPGameInstance()->GetCountMoveBack() - 1);

    // [Game Analytics] Send event back
    UGameAnalytics::AddDesignEvent("BackMove");
}

void UGameUserWidget::OnClickedPauseState()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    PlayAnimation(this->PauseButtonAnim);
    GetGamePlayMode()->OnChangeGameStateTimer(EGameState::Pause, DelayCloseButton);
}

void UGameUserWidget::ChangeCountCoin(int32 CountCoin)
{
    FSlateBrush NewBrash;
    NewBrash.SetResourceObject(LoaderUtils::LoaderSyncObject<UTexture2D>(GetWorld(), this->GoldTexture));
    NewBrash.SetImageSize(this->SizeGoldTexture);
    if (CountCoin == 1)
    {
        this->ImageOneCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageOneCoinAnim);
    }
    else if (CountCoin == 2)
    {
        this->ImageTwoCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageTwoCoinAnim);
    }
    else if (CountCoin == 3)
    {
        this->ImageThreeCoin->SetBrush(NewBrash);
        PlayAnimation(this->ImageThreeCoinAnim);
    }
}

void UGameUserWidget::UpdateTextCount(int32 NewCount)
{
    FSlateBrush SlateBrush;
    SlateBrush.SetResourceObject(LoaderUtils::LoaderSyncObject<UTexture2D>(GetWorld(), this->BackMoveTexture[NewCount]));
    SlateBrush.SetImageSize(this->SizeBackTexture);

    FButtonStyle ButtonStyle;
    ButtonStyle.SetNormal(SlateBrush);
    ButtonStyle.SetHovered(SlateBrush);
    ButtonStyle.SetPressed(SlateBrush);

    this->BackButton->SetStyle(ButtonStyle);
}

void UGameUserWidget::TeleportObject(APiratePawn* TempPirate, FVector NewLoc, FIntPoint NewPoint)
{
    // Effect teleport IN
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->TeleportOutEffect, NewLoc);

    // Stand Anim montage
    TempPirate->PlayAnimMontage(this->AnimStandPirate);

    TempPirate->SetNewPoint(NewPoint);
    TempPirate->SetActorLocation(NewLoc);
    TempPirate->ChangeStateBrain(EStateBrain::Idle);
    GetGamePlayMode()->StartAllSkeletonCannon();
    GetGamePlayMode()->StartAllSkeletonRunner();
}
