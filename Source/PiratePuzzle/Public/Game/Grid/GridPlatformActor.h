// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    // Getting position platform
    FIntPoint GetPositionPlatform() const { return (this->PlatformPos); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Getting pointer on current game mode
    AGamePlayMode* GetGamePlayMode() const { return (this->GameMode); }

    // Getting Base mesh platform
    UStaticMeshComponent* GetMeshPlatform() const { return (this->BaseMeshPlatform); };

private:
    // Pointer on Game mode
    AGamePlayMode* GameMode;

    // Main scene root components
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRoot;
    // Base mesh platform
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BaseMeshPlatform;
    // Debug position item on grid
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* TextDebugPosition;

    // Current Position on grid generator
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    FIntPoint PlatformPos = FIntPoint(-1, -1);

    friend class AGridGeneratorActor;
};
