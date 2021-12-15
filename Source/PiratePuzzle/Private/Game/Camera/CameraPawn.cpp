// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/Camera/CameraPawn.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Game/GamePlayMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/AI/Pirate/PiratePawn.h"
#include "Game/Grid/GridGeneratorActor.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Grid/GridPlatformActor.h"
#include "UtilsLib/BaseUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogCameraPawn, All, All);

// Sets default values
ACameraPawn::ACameraPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // Create root scene component
    this->SceneRootComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
    SetRootComponent(this->SceneRootComponent);

    // Create sphere
    this->SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere component");
    this->SphereComponent->SetupAttachment(this->SceneRootComponent);
    this->SphereComponent->SetSphereRadius(100.f);

    // Create spring arm component
    this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring component");
    this->SpringArmComponent->SetupAttachment(this->SphereComponent);
    this->SpringArmComponent->bDoCollisionTest = false;

    // Create camera
    this->CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
    this->CameraComponent->SetupAttachment(this->SpringArmComponent);

    // Create base pack rotation player
    this->BaseRotationPlayer.Add(EDirectionPlayer::Up, FInfoTrace(FRotator(0.f, 270.f, 0.f), FVector(0.f, 50.f, 0.f)));
    this->BaseRotationPlayer.Add(EDirectionPlayer::Down, FInfoTrace(FRotator(0.f, 90.f, 0.f), FVector(0.f, -50.f, 0.f)));
    this->BaseRotationPlayer.Add(EDirectionPlayer::Left, FInfoTrace(FRotator(0.f, 180.f, 0.f), FVector(50.f, 0.f, 0.f)));
    this->BaseRotationPlayer.Add(EDirectionPlayer::Right, FInfoTrace(FRotator(0.f, 0.f, 0.f), FVector(-50.f, 0.f, 0.f)));
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->SceneRootComponent, TEXT("Scene root component is nullptr"));
    checkf(this->SpringArmComponent, TEXT("Spring arm component is nullptr"));
    checkf(this->CameraComponent, TEXT("Camera component is nullptr"));
    checkf(this->SphereComponent, TEXT("Sphere component is nullptr"));

    // Set Default Rotation Camera
    this->RotDefaultCamera = this->SpringArmComponent->GetRelativeRotation();

    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game play mode is nullptr"));
}

void ACameraPawn::ChangeCameraPosition(float DeltaTime)
{
    if (this->TimeElyps < this->DefaultRateTime)
    {
        FRotator NewRot = FMath::Lerp(this->StartRot, this->EndRot, this->TimeElyps / this->DefaultRateTime);
        this->SpringArmComponent->SetRelativeRotation(NewRot);
        this->TimeElyps += DeltaTime;
        if (this->TimeElyps >= this->DefaultRateTime)
        {
            this->EnableAnimCamera = false;
            this->TimeElyps = 0.f;
            UE_LOG(LogCameraPawn, Display, TEXT("The camera successfully changed the view: %s"), *NewRot.ToString())
        }
    }
}

void ACameraPawn::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    if (FingerIndex != ETouchIndex::Touch1) return;
    this->StartTouch.X = Location.X;
    this->StartTouch.Y = Location.Y;
}

void ACameraPawn::OnTouchRepeat(ETouchIndex::Type FingerIndex, FVector Location)
{
    if (FingerIndex != ETouchIndex::Touch1) return;
    if (!this->AIPirate) return;
    if (this->GameMode->GetGameState() != EGameState::GameProgress) return;
    if (this->AIPirate->GetStateBrain() != EStateBrain::Idle) return;

    this->EndTouch.X = Location.X;
    this->EndTouch.Y = Location.Y;
    EDirectionPlayer NewDirection = this->UpdateDirectionForPlayer(this->StartTouch, this->EndTouch);
    if (this->DirectionPlayer != NewDirection)
    {
        this->DirectionPlayer = NewDirection;
        UE_LOG(LogCameraPawn, Display, TEXT("Update direction player: %s"), *UEnum::GetValueAsString(this->DirectionPlayer))
        this->AIPirate->SetActorRotation(this->BaseRotationPlayer[this->DirectionPlayer].RotationPlayer);
    }
}

void ACameraPawn::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    if (FingerIndex != ETouchIndex::Touch1) return;
    this->EndTouch.X = Location.X;
    this->EndTouch.Y = Location.Y;

    if (!this->AIPirate) return;
    if (this->GameMode->GetGameState() != EGameState::GameProgress) return;
    if (this->AIPirate->GetStateBrain() != EStateBrain::Idle) return;

    // Check register touch
    if (!this->CheckRegisterTouch(this->StartTouch, this->EndTouch)) return;

    FIntPoint NewPointLocation;
    if ((NewPointLocation = this->TryFindNewPointLocation()) == FIntPoint(-1, -1))
    {
        UE_LOG(LogCameraPawn, Error, TEXT("New point Location is zero: %s"), *NewPointLocation.ToString());
        return;
    }
    this->StartMoveAICharacterOnPos(NewPointLocation);
}

bool ACameraPawn::CheckRegisterTouch(const FVector2D& Start, const FVector2D& End)
{
    const auto LengthRegister = UKismetMathLibrary::VSize2D(FVector2D(End.X - Start.X, End.Y - Start.Y));

    if (LengthRegister < this->LenSizeTouch)
    {
        UE_LOG(LogCameraPawn, Error, TEXT("Length Register: %f less define len register:  %f"), LengthRegister, this->LenSizeTouch);
        return (false);
    }
    return (true);
}

void ACameraPawn::StartMoveAICharacterOnPos(FIntPoint NewPoint)
{
    if (this->AIPirate->GetStateBrain() == EStateBrain::Walk) return;

    // Adding one successful move
    this->GameMode->AddCountMoves();

    // Setting up a new position for a pirate
    auto TempMap = this->GameMode->GetGridGenerator()->GetMapPlatformOnGrid();
    FVector NewPos = BaseUtils::GetVectorPositionPlatform(NewPoint, TempMap);
    this->AIPirate->SetNewPoint(NewPoint);
    this->AIPirate->StartMovement(this->AIPirate->GetActorLocation(), NewPos);
}

void ACameraPawn::StartSwapCamera()
{
    if (this->AIPirate->GetStateBrain() != EStateBrain::Idle && this->AIPirate->GetStateBrain() != EStateBrain::Walk) return;
    if (this->EnableAnimCamera) return;

    this->StartRot = (this->IsCameraUp == true) ? this->RotUpCamera : this->RotDefaultCamera;
    this->EndRot = (this->IsCameraUp == true) ? this->RotDefaultCamera : this->RotUpCamera;
    this->IsCameraUp = (this->IsCameraUp == true) ? false : true;

    this->EnableAnimCamera = true;
    UE_LOG(LogCameraPawn, Display, TEXT("Start animation camera from %s to %s"), *this->StartRot.ToString(), *this->EndRot.ToString());
}

EDirectionPlayer ACameraPawn::UpdateDirectionForPlayer(const FVector2D& Start, const FVector2D& End)
{
    EDirectionPlayer NewDirectionPlayer;
    const auto NoAbsVector = FVector2D(Start.X - End.X, Start.Y - End.Y);
    const auto AbsVector = FVector2D(FMath::Abs(Start.X - End.X), FMath::Abs(Start.Y - End.Y));

    if (AbsVector.X > AbsVector.Y)  // left or right
    {
        NewDirectionPlayer = (NoAbsVector.X > 0) ? EDirectionPlayer::Left : EDirectionPlayer::Right;
    }
    else  // up or down
    {
        NewDirectionPlayer = (NoAbsVector.Y > 0) ? EDirectionPlayer::Up : EDirectionPlayer::Down;
    }
    return (NewDirectionPlayer);
}

FIntPoint ACameraPawn::TryFindNewPointLocation()
{
    FVector StartLine = this->AIPirate->GetCollision()->GetComponentLocation();
    FVector EndLine = StartLine + this->BaseRotationPlayer[this->DirectionPlayer].RotationPlayer.Vector() * this->DistanceTrace;

    FCollisionQueryParams FirstParams(FName(TEXT("param")), false, this->AIPirate);
    FCollisionObjectQueryParams FirstObjectParams(ECollisionChannel::ECC_WorldStatic);

    FHitResult FirstTrace = this->TryGetTrace(StartLine, EndLine, FirstParams, FirstObjectParams);
    if (FirstTrace.IsValidBlockingHit() && FirstTrace.GetActor())
    {
        const auto TempActor = FirstTrace.GetActor();
        UE_LOG(LogCameraPawn, Display, TEXT("Wall is actor: %s"), *TempActor->GetName());

        FVector BlockLocationStart = FirstTrace.ImpactPoint;
        BlockLocationStart += this->BaseRotationPlayer[this->DirectionPlayer].TraceLocation;
        FVector BlockLocationEnd = BlockLocationStart + (FRotator(-90.f, 0.f, 0.f)).Vector() * this->DistanceTrace;

        // Draw debug trace
        if (this->bEnableDebugTrace)
        {
            DrawDebugLine(GetWorld(), StartLine, BlockLocationStart, this->ColorTrace, false, this->TimeLifeTrace, 0, this->ThicknessTrace);
            DrawDebugSphere(GetWorld(), BlockLocationStart, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false,
                this->TimeLifeTrace, 0, this->ThicknessTrace);
        }
        FCollisionQueryParams SecondParams(FName(TEXT("param")), false, this->AIPirate);
        FCollisionObjectQueryParams SecondObjectParams(ECollisionChannel::ECC_WorldStatic);

        FHitResult SecondTrace = this->TryGetTrace(BlockLocationStart, BlockLocationEnd, SecondParams, SecondObjectParams);
        if (SecondTrace.IsValidBlockingHit() && SecondTrace.GetActor())
        {
            const auto TempSecondActor = SecondTrace.GetActor();
            UE_LOG(LogCameraPawn, Display, TEXT("Block item is actor: %s"), *TempSecondActor->GetName());
            // Draw debug trace
            if (this->bEnableDebugTrace)
            {
                DrawDebugLine(GetWorld(), BlockLocationStart, SecondTrace.ImpactPoint, this->ColorTrace, false, this->TimeLifeTrace, 0,
                    this->ThicknessTrace);
                DrawDebugSphere(GetWorld(), SecondTrace.ImpactPoint, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false,
                    this->TimeLifeTrace, 0, this->ThicknessTrace);
            }

            const auto TempPlatform = Cast<AGridPlatformActor>(TempSecondActor);
            if (!TempPlatform) return (FIntPoint(-1, -1));

            const FIntPoint TempPosPlatform = TempPlatform->GetPositionPlatform();
            const FIntPoint TempPosPirate = this->AIPirate->GetPointPosition();

            if (TempPosPlatform == TempPosPirate) return (FIntPoint(-1, -1));
            this->AIPirate->AddPreviousPointPosition(TempPosPirate);
            UE_LOG(LogCameraPawn, Display, TEXT("New Position AI pirate: %s"), *TempPosPlatform.ToString());
            return (TempPosPlatform);
        }
    }
    return (FIntPoint(-1, -1));
}

FHitResult ACameraPawn::TryGetTrace(
    FVector StartPos, FVector EndPos, FCollisionQueryParams Params, FCollisionObjectQueryParams ObjectParams)
{
    FHitResult HitResult;

    GetWorld()->LineTraceSingleByObjectType(HitResult, StartPos, EndPos, ObjectParams, Params);
    return (HitResult);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (this->EnableAnimCamera) this->ChangeCameraPosition(DeltaTime);
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindTouch(IE_Pressed, this, &ACameraPawn::OnTouchPressed);
        PlayerInputComponent->BindTouch(IE_Repeat, this, &ACameraPawn::OnTouchRepeat);
        PlayerInputComponent->BindTouch(IE_Released, this, &ACameraPawn::OnTouchReleased);
    }
}
