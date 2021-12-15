// Is the property of WerFEST Software

#pragma once

#include "CoreMinimal.h"
#include "AIDataTypes.h"
#include "PawnBase.generated.h"

class UPPGameInstance;
class UBoxComponent;
class AGamePlayMode;
UCLASS()
class PIRATEPUZZLE_API APawnBase : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    APawnBase();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Getting pawn skeletal mesh component
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting pawn skeletal mesh component"))
    USkeletalMeshComponent* GetMesh() const { return (this->BaseMesh); }

    // Getting collision component
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting collision component"))
    UBoxComponent* GetCollision() const { return (this->BoxCollision); }

    // Getting current Brain state
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting current Brain state"))
    const EStateBrain& GetStateBrain() const { return (this->StateBrain); }

    // Change pawn behavior
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Change pawn behavior"))
    virtual void ChangeStateBrain(EStateBrain NewState);

    // Getting Time move
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting Time move"))
    float GetTimeMove() const { return (this->TimeMove); }

    // Start Movement Pawn
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Start Movement Pawn"))
    virtual void StartMovement(FVector StartPoint, FVector EndPoint);

    // Stop Movement Pawn
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Stop Movement Pawn"))
    virtual void StopMovement();

    // The function of starting the timer for the next move
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "The function of starting the timer for the next move"))
    void StartMovementTimer(FVector StartPoint, FVector EndPoint, float RateTime);

    /** Play Animation Montage on the character mesh. Returns the length of the animation montage in seconds, or 0.f if failed to play. **/
    UFUNCTION(BlueprintCallable, Category = "APawnBase")
    virtual float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

    // Play Animation Montage on Timer
    UFUNCTION(BlueprintCallable, Category = "APawnBase")
    void PlayAnimMontageTimer(UAnimMontage* AnimMontage, float RateTime, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Get current class of AGamePlayMode
    UFUNCTION(BlueprintGetter, Category = "APawnBase")
    AGamePlayMode* GetGamePlayMode() const { return (this->GameMode); }

    // Get current class of AGamePlayMode
    UFUNCTION(BlueprintGetter, Category = "APawnBase")
    UPPGameInstance* GetPPGameInstance() const { return (this->GameInstance); }

    // Calculation of rotation in the direction of vector points
    FRotator CalculatePawnRotation(FVector StartPoint, FVector EndPoint);

    // Starting rotation at certain points
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Starting rotation at certain points"))
    void StartRotate(FRotator StartRotator, FRotator EndRotator);

    // Starting rotation at timer
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Starting rotation at timer"))
    void StartRotateTimer(FRotator StartRotator, FRotator EndRotator, float RateTime);

    /*
     * A set of parameters for displaying debug information
     */
    // Enable debug information?
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace", meta = (ToolTip = "Enable debug information?"))
    bool bEnableDebugInfo = false;
    // Color debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (ToolTip = "Color debug trace", EditCondition = "bEnableDebugInfo"))
    FColor ColorTrace = FColor::Red;
    // Life time debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (ToolTip = "Life time debug trace.", EditCondition = "bEnableDebugInfo"))
    float TimeLifeTrace = 5.f;
    // Thickness debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (ToolTip = "Thickness debug trace.", EditCondition = "bEnableDebugInfo"))
    float ThicknessTrace = 5.f;
    // Radius debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (ToolTip = "Radius debug sphere.", EditCondition = "bEnableDebugInfo"))
    float RadiusSphere = 5.f;
    // Segments debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (ToolTip = "Segments debug sphere.", EditCondition = "bEnableDebugInfo"))
    int32 SegmentsSphere = 5.f;

private:
    // Pointer on Game mode
    UPROPERTY()
    AGamePlayMode* GameMode;

    // Pointer on Game mode
    UPROPERTY()
    UPPGameInstance* GameInstance;

    // Scene root component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* RootScene;
    // A collision for a pawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UBoxComponent* BoxCollision;
    // Skeletal mesh component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* BaseMesh;

    /*
     * Parameters for controlling pawn movement and rotation
     */
    // Movement speed pawn cm/sec
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = true, ToolTip = "Movement speed pawn cm/sec", ClampMin = "100.0", ClampMax = "1500.0"))
    float DefaultSpeedMove = 1000.0f;

    // The zero state of the move lerp
    bool bMoveToPoint = false;
    // Vector of the starting point
    FVector StartPointPosition = FVector::ZeroVector;
    // Vector of the final position for movement
    FVector EndPointPosition = FVector::ZeroVector;
    // Intermediate value for time travel
    float TimeMoveEylips = 0.0f;
    // Time for move
    float TimeMove = 0.0f;

    // Speed Rotate pawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = true, ToolTip = "Speed Rotate pawn", ClampMin = "1.0", ClampMax = "10.0"))
    float DefaultSpeedRotate = 5.0f;

    // The zero state of the rotation lerp
    bool bRotateToPosition = false;
    // Vector of the starting point
    FRotator StartRotatePosition = FRotator::ZeroRotator;
    // Vector of the final position for movement
    FRotator EndRotatePosition = FRotator::ZeroRotator;
    // Intermediate value for Rotate travel
    float TimeRotateEylips = 0.0f;
    // Time for Rotate
    float TimeRotate = 0.0f;

    // Pawn Behavior States
    EStateBrain StateBrain = EStateBrain::None;

    // Timer handle for movement
    FTimerHandle TimerHandleMovement;

    // Timer handle for rotate
    FTimerHandle TimerHandleRotate;

    // Timer handle for Anim Montage
    FTimerHandle TimerHandleAnimMontage;

    // Function for moving to a given point
    void MoveToPoint(float DeltaTime);

    // Calculation of time depending on speed and overcoming distance
    float CalculateTime(float Distance, float Speed);

    // Function for smoothly changing the rotation position
    void RotateToPosition(float DeltaTime);
};
