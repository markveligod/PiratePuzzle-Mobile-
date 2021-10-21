// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/AI/AIDataTypes.h"
#include "GridGeneratorActor.generated.h"

class AGridTreasurePlatform;
class AGridQuicksandPlatform;
class AGridNeutralPlatform;
class AGridBarrierPlatform;
class AGridWallActor;
class AGridPlatformActor;
class APirateAICharacter;
class ASkeletonRunnerCharacter;
class AGoldActor;
UCLASS()
class PIRATEPUZZLE_API AGridGeneratorActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGridGeneratorActor();

    // Getting map position platform
    const TMap<FIntPoint, AGridPlatformActor*>& GetMapPlatformOnGrid() const { return (this->MapPlatformsOnGrid); }

    // Getting array pointer skeleton runners
    const TArray<ASkeletonRunnerCharacter*>& GetArraySkeletonRunners() const { return (this->ArraySkeletonRunners); }

    // Getting add Position on pirate in grid
    float GetPosPirateAxisZ() const { return (this->AddPiratePosZ); }

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
    TSubclassOf<AGridNeutralPlatform> SpawnNeutralPlatformRef;
    // Size grid
    UPROPERTY(
        EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform", meta = (AllowPrivateAccess = "true", ToolTip = "WidthCount"))
    int32 WidthCount = 6;
    UPROPERTY(
        EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform", meta = (AllowPrivateAccess = "true", ToolTip = "HeightCount"))
    int32 HeightCount = 6;
    // The distance between platforms when spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "The distance between platforms when spawning"))
    float DistancePlatform = 100.f;
    // Map Point and pointer spawn item on grid
    TMap<FIntPoint, AGridPlatformActor*> MapPlatformsOnGrid;

    // Barrier pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "Barrier pointer ref"))
    TSubclassOf<AGridBarrierPlatform> SpawnBarrierPlatformRef;
    // Barrier point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "Barrier point position for Spawn on platform"))
    TArray<FIntPoint> ArrayPosBarrier;

    // Quicksand pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Quicksand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Quicksand pointer ref"))
    TSubclassOf<AGridQuicksandPlatform> SpawnQuicksandPlatformRef;
    // Quicksand point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Quicksand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Quicksand point position for Spawn on platform"))
    TArray<FIntPoint> ArrayPosQuicksand;

    // Treasure pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Treasure",
        meta = (AllowPrivateAccess = "true", ToolTip = "Treasure pointer ref"))
    TSubclassOf<AGridTreasurePlatform> SpawnTreasurePlatformRef;
    // Treasure point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Treasure",
        meta = (AllowPrivateAccess = "true", ToolTip = "Treasure point position for Spawn on platform"))
    FIntPoint PosTreasure;

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

    /*
     * Part Settings for AI player
     */
    // Pirate pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Pirate pointer ref"))
    TSubclassOf<APirateAICharacter> SpawnPirateRef;
    // Pirate point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Pirate point position for Spawn on platform"))
    FIntPoint PosPirate;
    // Adds along the Z axis when a pirate spawns
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Adds along the Z axis when a pirate spawns"))
    float AddPiratePosZ = 50.f;
    // The angle of rotation when spawning a pirate
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "The angle of rotation when spawning a pirate"))
    float PirateRotZ = 0.f;
    // Pointer on pirate
    APirateAICharacter* AIPirate;

    /*
     * Part Settings for AI Skeleton runner
     */
    // Pirate pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Skeleton Runner pointer ref"))
    TSubclassOf<ASkeletonRunnerCharacter> SpawnSkeletonRunnerRef;
    // Array information for spawn Skeleton Runner
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Array information for spawn Skeleton Runner"))
    TArray<FInfoSpawnSkeletonRunner> ArrayInfoSpawnSkeletonRunners;
    // Adds along the Z axis when a pirate spawn
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Adds along the Z axis when a Skeleton Runner spawn"))
    float AddSkeletonRunnerPosZ = 50.f;
    // Pointers on skeleton runners
    TArray<ASkeletonRunnerCharacter*> ArraySkeletonRunners;

    /*
     * Part Settings for spawn Gold
     */
    // GoldActor pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Gold",
        meta = (AllowPrivateAccess = "true", ToolTip = "GoldActor pointer ref"))
    TSubclassOf<AGoldActor> SpawnGoldRef;
    // GoldActor point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Gold",
        meta = (AllowPrivateAccess = "true", ToolTip = "GoldActor point position for Spawn on platform"))
    TArray<FIntPoint> ArrayPosGold;
    // Adds along the Z axis when a GoldActor spawn
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Gold",
        meta = (AllowPrivateAccess = "true", ToolTip = "Adds along the Z axis when a GoldActor spawn"))
    float AddGoldPosAxisZ = 50.f;

    // Array pointers Gold Actor
    TArray<AGoldActor*> ArrayGolds;

    // Spawn platform on Grid
    void SpawnPlatform();

    // Checking for a specific platform's spawn
    AGridPlatformActor* SpawnSpecificPlatform(FIntPoint Point, FTransform SpawnTransform);

    // Spawn Wall on Grid
    void SpawnWall();

    // Spawn pirate on Grid
    void SpawnPirate();

    // Spawn Skeleton Runners
    void SpawnSkeletonRunners();

    // Spawn Gold on platform
    void SpawnGold();

    // Clear grid
    void ClearGrid();

    // Friend class game mode
    friend class AGamePlayMode;
};
