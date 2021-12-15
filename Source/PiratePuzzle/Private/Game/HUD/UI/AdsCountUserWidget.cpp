// Is the property of WerFEST Software

#include "Game/HUD/UI/AdsCountUserWidget.h"
#include "EasyAdsLibrary.h"
#include "GameAnalytics.h"
#include "PPGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/GamePlayMode.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAdsCountUserWidget, All, All);

void UAdsCountUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    this->AdsButton->OnClicked.AddDynamic(this, &UAdsCountUserWidget::ViewAds);
    this->CloseButton->OnClicked.AddDynamic(this, &UAdsCountUserWidget::CloseWidget);
}

void UAdsCountUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    this->ViewTimeAd();
}

void UAdsCountUserWidget::ViewAds()
{
    if (!GetPPGameInstance()->IsActiveAds()) return;

    // Activate rewarded video
    this->PlayVideoProxy = NewObject<UPlayVideoProxy>();
    this->PlayVideoProxy->OnSuccess.AddDynamic(this, &UAdsCountUserWidget::PlayVideoSuccess);
    if (UEasyAdsLibrary::IsRewardedVideoReady())
    {
        this->PlayVideoProxy->Activate();
        GetPPGameInstance()->ChangeActiveAds(false);
        UGameAnalytics::AddAdEvent(EGAAdAction::clicked, EGAAdType::rewardedvideo, "admob", "up_count");
        UE_LOG(LogAdsCountUserWidget, Display, TEXT("Click event Ads"));
    }

#if UE_EDITOR
    GetPPGameInstance()->SetupCountMoveBack(GetPPGameInstance()->GetDefaultCountMoveBack());
    this->CloseWidget();
#endif
}

void UAdsCountUserWidget::CloseWidget()
{
    GetGamePlayMode()->OnChangeGameState(EGameState::GameProgress);
}

void UAdsCountUserWidget::PlayVideoSuccess(FString Item, int32 Amount)
{
    UGameAnalytics::AddAdEvent(EGAAdAction::show, EGAAdType::rewardedvideo, "admob", "up_count");
    UE_LOG(LogAdsCountUserWidget, Display, TEXT("PlayVideoSuccess event Ads"));

    this->PlayVideoProxy->OnSuccess.Clear();
    this->PlayVideoProxy = nullptr;

    GetPPGameInstance()->SetupCountMoveBack(GetPPGameInstance()->GetDefaultCountMoveBack());
    this->CloseWidget();
}

void UAdsCountUserWidget::ViewTimeAd() const
{
    if (!GetPPGameInstance()->IsActiveAds())
    {
        const FString StrTime = BaseUtils::GetStringMinute(GetPPGameInstance()->GetCountDownAd());
        this->TimeAdTextBlock->SetText(FText::FromString(StrTime));
        this->TimeAdTextBlock->SetVisibility(ESlateVisibility::Visible);
        this->AdsButton->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        this->TimeAdTextBlock->SetText(FText::FromString(""));
        this->TimeAdTextBlock->SetVisibility(ESlateVisibility::Hidden);
        this->AdsButton->SetVisibility(ESlateVisibility::Visible);
    }
}
