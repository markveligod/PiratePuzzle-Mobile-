// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SharkCharacter.generated.h"

class ALabelTagActor;
class UBehaviorTree;
UCLASS()
class PIRATEPUZZLE_API ASharkCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASharkCharacter();

    // Get next location tag
    FVector GetNextPosTag();

    // Pointer on Behavior Tree
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tree")
    UBehaviorTree* BehaviorTreeAsset;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Constructor move position
    virtual void OnConstruction(const FTransform& Transform) override;

private:
    // A reference for creating a pointer to the location of the next location.
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "A reference for creating a pointer to the location of the next location."))
    TSubclassOf<ALabelTagActor> RefLabelTag;
    // Array of positions on which the shark will move
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "Array of positions on which the shark will move."))
    TArray<FVector> ArrayPosTags;

    // Current count position in Array
    int32 PosCountTag = 0;

    // Array tags
    TArray<ALabelTagActor*> ArrayTags;

    // Function clear tags
    void ClearTags();

    // Reset Default position
    void ResetArrayPosition();

    // Function Spawn tag on scene
    void SpawnTags();
};
