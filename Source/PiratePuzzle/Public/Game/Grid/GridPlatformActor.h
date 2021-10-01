// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridPlatformActor.generated.h"

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
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
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
    FVector2D PlatformPos = FVector2D(-1, -1);

    friend class AGridGeneratorActor;
};
