// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridQuicksandPlatform.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PiratePawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridQuicksandPlatform, All, All);

AGridQuicksandPlatform::AGridQuicksandPlatform()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());

    // Create effect quicksand
    this->EffectQuickSand = CreateDefaultSubobject<UNiagaraComponent>("Effect quicksand");
    this->EffectQuickSand->SetupAttachment(GetRootComponent());
}

void AGridQuicksandPlatform::BeginPlay()
{
    Super::BeginPlay();
    this->BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGridQuicksandPlatform::RegisterCollisionOverlap);
}

void AGridQuicksandPlatform::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (this->bEnableDepth) this->DiveAIPirate(DeltaSeconds);
}

void AGridQuicksandPlatform::DiveAIPirate(float DeltaTime)
{
    if (this->TimeElyps < this->RateTimeSand)
    {
        const float NewAxisZ = FMath::Lerp(this->StartPos, this->EndPos, this->TimeElyps / this->RateTimeSand);
        FVector NewLoc = this->SuicideBomber->GetActorLocation();
        NewLoc.Z = NewAxisZ;
        this->SuicideBomber->SetActorLocation(NewLoc);
        this->TimeElyps += DeltaTime;
        if (this->TimeElyps >= this->RateTimeSand)
        {
            this->TimeElyps = 0.f;
            this->SuicideBomber = nullptr;
            this->bEnableDepth = false;
        }
    }
}

void AGridQuicksandPlatform::RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogGridQuicksandPlatform, Warning, TEXT("Call Register without param OtherActor"));
        return;
    }
    UE_LOG(LogGridQuicksandPlatform, Display, TEXT("Name Platform: %s | Position: %s | Name Actor overlap: %s"), *GetName(),
        *GetPositionPlatform().ToString(), *OtherActor->GetName());
    if (OtherActor->IsA(APiratePawn::StaticClass()))
    {
        APiratePawn* TempPirate = Cast<APiratePawn>(OtherActor);
        TempPirate->StopMovement();
        TempPirate->SetActorLocation(this->BoxCollision->GetComponentLocation());
        TempPirate->GetCollision()->SetCollisionProfileName("NoCollision");
        TempPirate->ChangeStateBrain(EStateBrain::Sand);

        this->SuicideBomber = TempPirate;
        this->StartPos = TempPirate->GetActorLocation().Z;
        this->EndPos = this->StartPos - this->Depth;
        this->bEnableDepth = true;
        GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameOver, RateTimeSand);
    }
}
