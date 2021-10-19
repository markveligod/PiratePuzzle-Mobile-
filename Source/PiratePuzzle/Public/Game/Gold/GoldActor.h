// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldActor.generated.h"

class USphereComponent;
UCLASS()
class PIRATEPUZZLE_API AGoldActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGoldActor();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* RootScene;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* GoldMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USphereComponent* SphereCollision;
};
