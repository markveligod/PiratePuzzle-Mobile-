// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/Grid/GridPlatformActor.h"
#include "GridBarrierPlatform.generated.h"

class UBoxComponent;

/**
 *
 */
UCLASS()
class PIRATEPUZZLE_API AGridBarrierPlatform : public AGridPlatformActor
{
    GENERATED_BODY()

public:
    AGridBarrierPlatform();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // Box collision barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;
    // Base Mesh Component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BaseMeshBarrier;

    // Array Static mesh for barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "A set of meshes for displaying the barrier"))
    TArray<UStaticMesh*> ArrayMeshBarrier;
    // Array Rotation for barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "A set of turns to display the barrier"))
    TArray<FRotator> ArrayRotationBarrier;
};
