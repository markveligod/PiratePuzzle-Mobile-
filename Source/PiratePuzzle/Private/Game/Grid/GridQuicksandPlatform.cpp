// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridQuicksandPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/GamePlayMode.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridQuicksandPlatform, All, All);

AGridQuicksandPlatform::AGridQuicksandPlatform()
{
    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());
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
        float NewAxisZ = FMath::Lerp(this->StartPos, this->EndPos, this->TimeElyps / this->RateTimeSand);
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
    if (OtherActor->IsA(APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* TempPirate = Cast<APirateAICharacter>(OtherActor);
        TempPirate->SetStateAI(EStateAI::DeathSand);
        TempPirate->GetCharacterMovement()->StopActiveMovement();
        TempPirate->SetActorLocation(this->BoxCollision->GetComponentLocation());
        TempPirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TempPirate->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

        this->SuicideBomber = TempPirate;
        this->StartPos = TempPirate->GetActorLocation().Z;
        this->EndPos = this->StartPos - this->Depth;
        this->bEnableDepth = true;
        GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameOver);
    }
}
