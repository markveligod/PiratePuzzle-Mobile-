// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/GameDataTypes.h"
#include "GameFramework/Actor.h"
#include "Game/Grid/GridDataTypes.h"
#include "GridPlatformActor.generated.h"

class AGamePlayMode;
class UTextRenderComponent;
UCLASS()
class PIRATEPUZZLE_API AGridPlatformActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGridPlatformActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Getting position platform
    FIntPoint GetPositionPlatform() const { return (this->PlatformPos); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Getting pointer on current game mode
    AGamePlayMode* GetGamePlayMode() const { return (this->GameMode); }

private:
    // Pointer on Game mode
    AGamePlayMode* GameMode;

    // Main scene root components
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRoot;
    // Static mesh item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMeshComponent;
    // Debug position item on grid
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* TextDebugPosition;

    // Current Position on grid generator
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    FIntPoint PlatformPos = FIntPoint(-1, -1);

    friend class AGridGeneratorActor;
};
