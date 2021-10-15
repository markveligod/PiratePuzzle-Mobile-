// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridQuicksandPlatform.h"
#include "Components/BoxComponent.h"
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
        APirateAICharacter* AIPirate = Cast<APirateAICharacter>(OtherActor);
        AIPirate->SetStateAI(EStateAI::DeathSand);
        AIPirate->GetCharacterMovement()->StopActiveMovement();
        AIPirate->SetActorLocation(this->BoxCollision->GetComponentLocation());
        OnChangeStateTimer(EGameState::GameOver, this->RateTime);
    }
}
