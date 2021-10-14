// Pirate Puzzle. Contact: markveligod@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Game/Camera/CameraDataTypes.h"
#include "CameraPawn.generated.h"

class AGamePlayMode;
class APirateAICharacter;
class USpringArmComponent;
class UCameraComponent;
class USphereComponent;
UCLASS()
class PIRATEPUZZLE_API ACameraPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ACameraPawn();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Set AI pirate
    void SetAIPirate(APirateAICharacter* Pirate) { this->AIPlayer = Pirate; }

    // Run move ai character
    void StartMoveAICharacterOnPos(FIntPoint NewPoint);

    // Function for starting the camera change
    UFUNCTION(BlueprintCallable)
    void StartSwapCamera();

    // Getting pointer on AI pirate
    APirateAICharacter* GetAIPirate() const { return (this->AIPlayer); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    AGamePlayMode* GameMode;

    // root Scene component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRootComponent;
    // Sphere pre-made
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereComponent;
    // Camera player
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;
    // Distance for camera
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

    // Size register touch control
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Touch",
        meta = (AllowPrivateAccess = "true", ToolTip = "Size register touch control"))
    float LenSizeTouch = 100.f;
    // Distance general trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Touch",
        meta = (AllowPrivateAccess = "true", ToolTip = "Distance general trace"))
    float DistanceTrace = 1000.f;

    // starting position of the finger
    FVector2D StartTouch;
    // end position of the finger
    FVector2D EndTouch;
    // Calculated player direction after calling the touch event
    EDirectionPlayer DirectionPlayer;
    // Map base rotation player for trace calculate direction
    TMap<EDirectionPlayer, FInfoTrace> BaseRotationPlayer;
    // Pointer ai player for controlled
    APirateAICharacter* AIPlayer;

    /*
     * Debug line trace from pirate
     */
    // Enable debug rendering of pirate movement lines.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Enable debug rendering of pirate movement lines."))
    bool bEnableDebugTrace = true;
    // Color debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Color debug trace.", EditCondition = "bEnableDebugTrace"))
    FColor ColorTrace = FColor::Red;
    // Life time debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Life time debug trace.", EditCondition = "bEnableDebugTrace"))
    float TimeLifeTrace = 5.f;
    // Thickness debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Thickness debug trace.", EditCondition = "bEnableDebugTrace"))
    float ThicknessTrace = 5.f;
    // Radius debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Radius debug sphere.", EditCondition = "bEnableDebugTrace"))
    float RadiusSphere = 5.f;
    // Segments debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Segments debug sphere.", EditCondition = "bEnableDebugTrace"))
    int32 SegmentsSphere = 5.f;

    /*
     * Settings animation camera swap
     */
    // Camera relative rotation from above
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim Camera",
        meta = (AllowPrivateAccess = "true", ToolTip = "Camera relative rotation from above."))
    FRotator RotUpCamera = FRotator::ZeroRotator;
    // Time to change the camera in seconds
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Anim Camera",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time to change the camera in seconds."))
    float DefaultRateTime = 1.f;
    // Default camera relative rotation
    FRotator RotDefaultCamera;
    // State Camera is Upper
    bool IsCameraUp = false;
    // State Start Animation Camera
    bool EnableAnimCamera = false;
    // Start rotation value
    FRotator StartRot;
    // End rotation value
    FRotator EndRot;
    // Intermediate values for the alpha channel
    float TimeElyps = 0.f;

    // Camera change function
    void ChangeCameraPosition(float DeltaTime);

    // Pack function for register touch of the finger
    void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);
    void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

    // Calculation of values for determining the current direction of the player
    bool UpdateDirectionForPlayer();
    // Find new Location
    FIntPoint TryFindNewPointLocation();
    // TryGetTrace
    FHitResult TryGetTrace(FVector StartPos, FVector EndPos, FCollisionQueryParams Params, FCollisionObjectQueryParams ObjectParams);

    friend class AGameHUD;
};
