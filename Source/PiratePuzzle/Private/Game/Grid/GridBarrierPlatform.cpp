// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Grid/GridBarrierPlatform.h"
#include "Components/BoxComponent.h"
#include "UtilsLib/LoaderUtils.h"

AGridBarrierPlatform::AGridBarrierPlatform()
{
    // Create Box Collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision component");
    this->BoxCollision->SetupAttachment(GetRootComponent());
    this->BoxCollision->SetCollisionProfileName("BlockAll");

    // Create Base Mesh component
    this->BaseMeshBarrier = CreateDefaultSubobject<UStaticMeshComponent>("Base barrier static mesh");
    this->BaseMeshBarrier->SetupAttachment(GetRootComponent());
    this->BaseMeshBarrier->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGridBarrierPlatform::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->BoxCollision, TEXT("Box collision is nullptr"));
    checkf(this->BaseMeshBarrier, TEXT("Mesh Barrier is nullptr"));
    checkf(this->ArrayMeshBarrier.Num() != 0, TEXT("Array Barrier size is 0"));
    checkf(this->ArrayRotationBarrier.Num() != 0, TEXT("Array Barrier size is 0"));

    // Random Load and Set Mesh Barrier
    UStaticMesh* NewMeshBarrier = LoaderUtils::LoaderSyncObject<UStaticMesh>(
        GetWorld(), this->ArrayMeshBarrier[FMath::RandRange(0, this->ArrayMeshBarrier.Num() - 1)]);
    this->BaseMeshBarrier->SetStaticMesh(NewMeshBarrier);

    // Random Rotation Mesh Barrier
    this->BaseMeshBarrier->SetRelativeRotation(this->ArrayRotationBarrier[FMath::RandRange(0, this->ArrayRotationBarrier.Num() - 1)]);
}

void AGridBarrierPlatform::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    // Random Load and Set Mesh Barrier
    UStaticMesh* NewMeshBarrier = LoaderUtils::LoaderSyncObject<UStaticMesh>(
        GetWorld(), this->ArrayMeshBarrier[FMath::RandRange(0, this->ArrayMeshBarrier.Num() - 1)]);
    this->BaseMeshBarrier->SetStaticMesh(NewMeshBarrier);

    // Random Rotation Mesh Barrier
    this->BaseMeshBarrier->SetRelativeRotation(this->ArrayRotationBarrier[FMath::RandRange(0, this->ArrayRotationBarrier.Num() - 1)]);
}
