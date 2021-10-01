// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Game/Grid/GridPlatformActor.h"
#include "GridQuicksandPlatform.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PIRATEPUZZLE_API AGridQuicksandPlatform : public AGridPlatformActor
{
    GENERATED_BODY()

public:
    AGridQuicksandPlatform();
    
protected:
    virtual void BeginPlay() override;
    
private:
    // Box collision for register begin overlap with Pirate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;

    
};
