// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Gold/GoldActor.h"
#include "Game/GamePlayMode.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "UtilsLib/BaseUtils.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PPGameInstance.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogGoldActor, All, All);

// Sets default values
AGoldActor::AGoldActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Root Scene component
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
    SetRootComponent(this->RootScene);

    // Create collision component
    this->SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere component");
    this->SphereCollision->SetupAttachment(this->RootScene);

    // Create Visual effect coin
    this->EffectCoin = CreateDefaultSubobject<UNiagaraComponent>("Effect coin");
    this->EffectCoin->SetupAttachment(this->RootScene);
}

// Called when the game starts or when spawned
void AGoldActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->EffectCoin, TEXT("Visual effect coin is nullptr"));
    checkf(this->SphereCollision, TEXT("Sphere collision is nullptr"));
    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameInstance = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
    checkf(this->GameInstance, TEXT("Game instance is nullptr"));

    this->GlobalStartLocation = this->GlobalEndLocation = GetActorLocation();
    this->GlobalEndLocation.Z += this->AddMovementAxisZ;

    this->SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AGoldActor::OnRegisterBeginOverlap);

    // Test spawn niagara
    if (this->GameInstance->GetRunLevel() == 1)
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->DestructionEffect, GetActorLocation());
}

void AGoldActor::MoveLocationCoin(float DeltaTime)
{
    FVector Location = GetActorLocation();
    // General size
    const float JourneyLength = (this->GlobalEndLocation - this->GlobalStartLocation).Size();
    // Size from start location to current location cube
    const float JourneyTravelled = (Location - GlobalStartLocation).Size();
    const float HalfSize = JourneyLength / 2.f;

    // if current location current platform > general size => Swap
    if (JourneyTravelled >= JourneyLength) BaseUtils::SwapData<FVector>(this->GlobalStartLocation, this->GlobalEndLocation);
    const FVector DirectionMove = (GlobalEndLocation - GlobalStartLocation).GetSafeNormal();

    const float Ratio = (FMath::Abs<float>(JourneyTravelled - HalfSize)) / HalfSize;
    // UE_LOG(LogGoldActor, Display, TEXT("Name Gold: %s | Ratio: %f | Journey Travelled: %f | HalfSize: %f"), *GetName(), Ratio,
    // JourneyTravelled, HalfSize);
    const float Speed = FMath::Lerp(this->SpeedMove.Min, this->SpeedMove.Max, Ratio);

    Location += Speed * DeltaTime * DirectionMove;
    SetActorLocation(Location);
}

// Called every frame
void AGoldActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    this->MoveLocationCoin(DeltaTime);
    if (this->EnableDebugInfo)
    {
        DrawDebugSphere(GetWorld(), this->GlobalStartLocation, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace);
        DrawDebugLine(
            GetWorld(), this->GlobalStartLocation, this->GlobalEndLocation, this->ColorTrace, false, -1.f, 0, this->ThicknessTrace);
        DrawDebugSphere(GetWorld(), this->GlobalEndLocation, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace);
    }
}

void AGoldActor::OnRegisterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogGoldActor, Warning, TEXT("Name Gold: %s | Call register overlap with OtherActor == nullptr"), *GetName());
        return;
    }

    // Pirate overlap
    if (OtherActor->IsA(APiratePawn::StaticClass()))
    {
        APiratePawn* Pirate = Cast<APiratePawn>(OtherActor);
        Pirate->PlayAnimMontage(this->TakeGoldAnim);
        this->GameMode->IncreaseCountCoin();
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->DestructionEffect, GetActorLocation());
        UGameplayStatics::PlaySound2D(GetWorld(), this->SoundCoin);
        Destroy();
    }
}
