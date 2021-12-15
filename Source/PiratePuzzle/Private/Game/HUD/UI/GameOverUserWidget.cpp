// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/HUD/UI/GameOverUserWidget.h"
#include "EasyAdsLibrary.h"
#include "GameAnalytics.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UtilsLib/BaseUtils.h"
#include "UtilsLib/LoaderUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameOverUserWidget, All, All);

void UGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->RestartButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::OnClickedRestart);
    this->BackButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::OnClickedAds);
    this->HomeButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::OnClickedHome);

    this->GameStateDelegateHandle = GetGamePlayMode()->OnGameStateChanged.AddUObject(this, &UGameOverUserWidget::OnChangeGameState);

    this->UpdateImageMoveBackCount(GetPPGameInstance()->GetCountMoveBack());
    GetPPGameInstance()->CountMoveBackChanged.AddUObject(this, &UGameOverUserWidget::UpdateImageMoveBackCount);

    UE_LOG(LogGameOverUserWidget, Display, TEXT("Native On Initialized"));
}

void UGameOverUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    this->ViewTimeAd();
}

void UGameOverUserWidget::OnClickedRestart()
{
    PlayAnimation(this->RestartButtonAnim);

    const FName NameLevel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
    OpenLevelTimer(NameLevel, this->RestartButtonAnim->GetEndTime());
}

void UGameOverUserWidget::OnClickedAds()
{
    if (!GetStateActiveButton()) return;
    DisableButtonActive();
    ButtonActiveTimer(true, this->MoveBackButtonAnim->GetEndTime());

    PlayAnimation(this->MoveBackButtonAnim);

    if (GetPPGameInstance()->GetCountMoveBack() == 0)
    {
        if (!GetPPGameInstance()->IsActiveAds()) return;

        // Activate rewarded video
        UPlayVideoProxy* PlayVideoProxy = NewObject<UPlayVideoProxy>();
        PlayVideoProxy->OnSuccess.AddDynamic(this, &UGameOverUserWidget::PlayVideoSuccess);
        if (UEasyAdsLibrary::IsRewardedVideoReady())
        {
            PlayVideoProxy->Activate();
            UGameAnalytics::AddAdEvent(EGAAdAction::clicked, EGAAdType::rewardedvideo, "admob", "rollback_move");
        }
#if UE_EDITOR
        GetPPGameInstance()->SetupCountMoveBackTimer(
            GetPPGameInstance()->GetDefaultCountMoveBack() - 1, this->MoveBackButtonAnim->GetEndTime() / 2.0f);
        GetGamePlayMode()->ResetDead();
#endif
        return;
    }

    GetPPGameInstance()->SetupCountMoveBackTimer(
        GetPPGameInstance()->GetCountMoveBack() - 1, this->MoveBackButtonAnim->GetEndTime() / 2.0f);
    GetGamePlayMode()->ResetDead();
}

void UGameOverUserWidget::OnClickedHome()
{
    ShowAnim(this->HomeButtonAnim);
    OpenLevelTimer(GetPPGameInstance()->GetNameMenuLevel(), this->HomeButtonAnim->GetEndTime());
}

void UGameOverUserWidget::OnChangeGameState(EGameState NewState)
{
    if (NewState == EGameState::GameProgress)
    {
        const FDelegateHandle DelegateHandle = GetGamePlayMode()->GetCameraPawn()->GetAIPirate()->OnStateBrainChanged.AddUObject(
            this, &UGameOverUserWidget::OnChangeStateBrainPirate);
        GetGamePlayMode()->OnGameStateChanged.Remove(this->GameStateDelegateHandle);
        this->GameStateDelegateHandle.Reset();

        UE_LOG(LogGameOverUserWidget, Display, TEXT("OnStateBrainChanged is done | GameStateDelegate is valid: %s"),
            *BaseUtils::GetStringFromBool(DelegateHandle.IsValid()));
    }
}

void UGameOverUserWidget::OnChangeStateBrainPirate(EStateBrain NewStateBrain)
{
    UMaterialInstance* TempMaterial = nullptr;
    UE_LOG(LogGameOverUserWidget, Display, TEXT("A new state of the brain has arrived: %s"), *UEnum::GetValueAsString(NewStateBrain));

    switch (NewStateBrain)
    {
        case EStateBrain::Sand: TempMaterial = this->LoseQuickSandMaterial; break;
        case EStateBrain::FellSkeleton: TempMaterial = this->LoseSkeletonSandMaterial; break;
        case EStateBrain::FellCannon: TempMaterial = this->LoseCannonSandMaterial; break;
        default: break;
    }
    if (TempMaterial)
    {
        FSlateBrush SlateBrush;
        SlateBrush.SetResourceObject(TempMaterial);
        SlateBrush.SetImageSize(this->DefaultSizeMaterial);
        ImageFlipBook->SetBrush(SlateBrush);
    }
}

void UGameOverUserWidget::UpdateImageMoveBackCount(int32 NewCount)
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

void UGameOverUserWidget::PlayVideoSuccess(FString Item, int32 Amount)
{
    UGameAnalytics::AddAdEvent(EGAAdAction::show, EGAAdType::rewardedvideo, "admob", "rollback_move");

    GetPPGameInstance()->SetupCountMoveBackTimer(GetPPGameInstance()->GetDefaultCountMoveBack() - 1, GetEndAnim()->GetEndTime());
    GetGamePlayMode()->ResetDead();
}

void UGameOverUserWidget::ViewTimeAd() const
{
    if (!GetPPGameInstance()->IsActiveAds() && GetPPGameInstance()->GetCountMoveBack() == 0)
    {
        const FString StrTime = BaseUtils::GetStringMinute(GetPPGameInstance()->GetCountDownAd());
        this->TimeAdTextBlock->SetText(FText::FromString(StrTime));
        this->TimeAdTextBlock->SetVisibility(ESlateVisibility::Visible);
        this->BackButton->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        this->TimeAdTextBlock->SetText(FText::FromString(""));
        this->TimeAdTextBlock->SetVisibility(ESlateVisibility::Hidden);
        this->BackButton->SetVisibility(ESlateVisibility::Visible);
    }
}
