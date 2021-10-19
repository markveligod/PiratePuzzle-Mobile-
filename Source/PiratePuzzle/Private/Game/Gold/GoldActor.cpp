// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Gold/GoldActor.h"

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogGoldActor, All, All);

// Sets default values
AGoldActor::AGoldActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create Root Scene component
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
    SetRootComponent(this->RootScene);

    // Create base static mesh component
    this->GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("Gold Static mesh");
    this->GoldMesh->SetupAttachment(this->RootScene);

    // Create collision component
    this->SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere component");
    this->SphereCollision->SetupAttachment(this->RootScene);
}

// Called when the game starts or when spawned
void AGoldActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->GoldMesh, TEXT("Gold mesh is nullptr"));
    checkf(this->SphereCollision, TEXT("Sphere collision is nullptr"));

    this->GlobalStartLocation = this->GlobalEndLocation = GetActorLocation();
    this->GlobalEndLocation.Z += this->AddMovementAxisZ;
}

void AGoldActor::RunRotationCoin(float DeltaTime)
{
    this->GoldMesh->AddRelativeRotation(FRotator(0.f, this->PowerRotYawCoin * DeltaTime, 0.f));
}

void AGoldActor::MoveLocationCoin(float DeltaTime)
{
    FVector Location = GetActorLocation();
    // General size
    float JourneyLength = (this->GlobalEndLocation - this->GlobalStartLocation).Size();
    // Size from start location to current location cube
    float JourneyTravelled = (Location - GlobalStartLocation).Size();
    float HalfSize = JourneyLength / 2.f;

    // if current location current platform > general size => Swap
    if (JourneyTravelled >= JourneyLength) BaseUtils::SwapData<FVector>(this->GlobalStartLocation, this->GlobalEndLocation);
    FVector DirectionMove = (GlobalEndLocation - GlobalStartLocation).GetSafeNormal();

    float Ratio = (FMath::Abs<float>(JourneyTravelled - HalfSize)) / HalfSize;
    // UE_LOG(LogGoldActor, Display, TEXT("Name Gold: %s | Ratio: %f | Journey Travelled: %f | HalfSize: %f"), *GetName(), Ratio,
    // JourneyTravelled, HalfSize);
    float Speed = FMath::Lerp(this->SpeedMove.Min, this->SpeedMove.Max, Ratio);

    Location += Speed * DeltaTime * DirectionMove;
    SetActorLocation(Location);
}

// Called every frame
void AGoldActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    this->RunRotationCoin(DeltaTime);
    this->MoveLocationCoin(DeltaTime);
    if (this->EnableDebugInfo)
    {
        DrawDebugSphere(GetWorld(), this->GlobalStartLocation, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace);
        DrawDebugLine(
            GetWorld(), this->GlobalStartLocation, this->GlobalEndLocation, this->ColorTrace, false, -1.f, 0, this->ThicknessTrace);
        DrawDebugSphere(GetWorld(), this->GlobalEndLocation, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace);
    }
}
