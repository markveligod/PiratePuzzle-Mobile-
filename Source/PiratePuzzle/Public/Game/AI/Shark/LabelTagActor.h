// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabelTagActor.generated.h"

UCLASS()
class PIRATEPUZZLE_API ALabelTagActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ALabelTagActor();

    // Getting position tag
    FVector GetPositionTag() const { return (GetActorLocation()); }
    // Setting position tag
    void SetPositionTag(FVector NewPos) { SetActorLocation(NewPos); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    // Root component
    UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    USceneComponent* RootScene;

    // Debug meta for draw in viewport editor
    UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* DebugMeshComponent;
};
