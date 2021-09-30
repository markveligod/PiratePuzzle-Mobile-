// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridGeneratorActor.generated.h"

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
    UStaticMeshComponent* StaticMeshHandleComponent;

    // Size grid
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings Spawn Grid", meta = (AllowPrivateAccess = "true", ToolTip = "WidthCount"))
    int32 WidthCount = 6;
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings Spawn Grid", meta = (AllowPrivateAccess = "true", ToolTip = "HeightCount"))
    int32 HeightCount = 6;

    
};
