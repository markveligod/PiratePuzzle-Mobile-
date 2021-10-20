// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/GameDataTypes.h"
#include "GoldActor.generated.h"

class UNiagaraSystem;
class AGamePlayMode;
class USphereComponent;
class UNiagaraComponent;
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
    // Current Game mode
    AGamePlayMode* GameMode;
    
    // Root Scene component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* RootScene;
    // Sphere Collision for register begin overlap
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USphereComponent* SphereCollision;
    // The visual effect of the coin
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UNiagaraComponent* EffectCoin;
    
    /*
     * A set of parameters for determining the internal animation of a coin
     */
    // Destruction Effect
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Destruction Effect."))
    UNiagaraSystem* DestructionEffect;
    // How high will the path be on the Z axis
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "How high will the path be on the Z axis."))
    float AddMovementAxisZ = 50.f;
    // Coin movement speed
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "How high will the path be on the Z axis."))
    FIntervalFloat SpeedMove{20.f, 100.f};

    // Enable debug information
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Enable debug inforamtion."))
    bool EnableDebugInfo = true;
    // Color debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Color debug trace.", EditCondition = "EnableDebugInfo"))
    FColor ColorTrace = FColor::Red;
    // Thickness debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Thickness debug trace.", EditCondition = "EnableDebugInfo"))
    float ThicknessTrace = 5.f;
    // Radius debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Radius debug sphere.", EditCondition = "EnableDebugInfo"))
    float RadiusSphere = 5.f;
    // Segments debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Gold",
        meta = (AllowPrivateAccess = true, ToolTip = "Segments debug sphere.", EditCondition = "EnableDebugInfo"))
    int32 SegmentsSphere = 5;

    FVector GlobalStartLocation;
    FVector GlobalEndLocation;

    // The function of moving the class along the Z axis
    void MoveLocationCoin(float DeltaTime);

    // Function for registering a collision intersection
    UFUNCTION()
    void OnRegisterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};


