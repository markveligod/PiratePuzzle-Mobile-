// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridGeneratorActor.generated.h"

class AGridWallActor;
class AGridPlatformActor;
UCLASS()
class PIRATEPUZZLE_API AGridGeneratorActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGridGeneratorActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // constructor
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // Root component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* RootScene;
    // Static mesh for root component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMeshHandleComponent;

    /*
    * Part Settings Spawn Platform Grid
    */
    // Reference for mesh spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "Reference for mesh spawning"))
    TSubclassOf<AGridPlatformActor> SpawnPlatformRef;
    // Size grid
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "WidthCount"))
    int32 WidthCount = 6;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "HeightCount"))
    int32 HeightCount = 6;
    // The distance between platforms when spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "The distance between platforms when spawning"))
    float DistancePlatform = 100.f;
    // Map Point and pointer spawn item on grid
    TMap<FIntPoint, AGridPlatformActor*> MapPlatformsOnGrid;

    /*
     * Part Settings Spawn Wall on Grid
     */
    // Reference for Wall spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Wall",
        meta = (AllowPrivateAccess = "true", ToolTip = "Reference for Wall spawning"))
    TSubclassOf<AGridWallActor> SpawnWallRef;
    // Size Wall
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Wall",
        meta = (AllowPrivateAccess = "true", ToolTip = "X -> Wall width Y -> Wall height"))
    FVector2D SizeWall = FVector2D::ZeroVector;
    // Array pointer spawn wall on grid
    TArray<AGridWallActor*> WallsGrid;

    
    // Spawn platform on Grid
    void SpawnPlatform();

    // Spawn Wall on Grid
    void SpawnWall();
    
    // Clear grid
    void ClearGrid();
};
