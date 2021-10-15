// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridTreasurePlatform.h"
#include "Components/BoxComponent.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGridTreasurePlatform, All, All);

AGridTreasurePlatform::AGridTreasurePlatform()
{
    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());
}

void AGridTreasurePlatform::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->BoxCollision, TEXT("Box collision component is nullptr"));
    this->BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AGridTreasurePlatform::RegisterCollisionOverlap);
}

void AGridTreasurePlatform::RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogGridTreasurePlatform, Warning, TEXT("Call Register without param OtherActor"));
        return;
    }
    UE_LOG(LogGridTreasurePlatform, Display, TEXT("Name Platform: %s | Position: %s | Name Actor overlap: %s"), *GetName(),
        *GetPositionPlatform().ToString(), *OtherActor->GetName());
    if (OtherActor->IsA(APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* AIPirate = Cast<APirateAICharacter>(OtherActor);
        AIPirate->SetStateAI(EStateAI::Win);
        AIPirate->GetCharacterMovement()->StopActiveMovement();
        AIPirate->SetActorLocation(this->BoxCollision->GetComponentLocation());
        OnChangeStateTimer(EGameState::GameWin, this->RateTime);
    }
}
