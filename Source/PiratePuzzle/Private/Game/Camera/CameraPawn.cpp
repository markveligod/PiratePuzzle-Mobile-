// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Camera/CameraPawn.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Game/AI/Pirate/PirateAICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Grid/GridPlatformActor.h"

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

}

void ACameraPawn::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    this->StartTouch.X = Location.X;
    this->StartTouch.Y = Location.Y;
}

void ACameraPawn::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    this->EndTouch.X = Location.X;
    this->EndTouch.Y = Location.Y;

    if (!this->AIPlayer) return;
    
    if (!this->UpdateDirectionForPlayer()) return;

    FVector NewPointLocation;
    if ((NewPointLocation = this->TryFindNewPointLocation()).IsZero())
    {
        UE_LOG(LogCameraPawn, Error, TEXT("New point Location is zero: %s"), *NewPointLocation.ToString());
        return;
    }
    this->StartMoveAICharacterOnPos(NewPointLocation);
}

void ACameraPawn::StartMoveAICharacterOnPos(FVector NewPos)
{
    this->AIPlayer->SetNextLocation(NewPos);
    this->AIPlayer->SetStateAI(EStateAI::Walk);
}

bool ACameraPawn::UpdateDirectionForPlayer()
{
    const auto NoAbsVector = FVector2D(this->StartTouch.X - this->EndTouch.X, this->StartTouch.Y - this->EndTouch.Y);
    const auto AbsVector = FVector2D(FMath::Abs(this->StartTouch.X - this->EndTouch.X), FMath::Abs(this->StartTouch.Y - this->EndTouch.Y));
    const auto LengthRegister = UKismetMathLibrary::VSize2D(FVector2D(this->EndTouch.X - this->StartTouch.X, this->EndTouch.Y - this->StartTouch.Y));

    if (LengthRegister < this->LenSizeTouch)
    {
        UE_LOG(LogCameraPawn, Error, TEXT("Length Register: %f less define len register:  %f"), LengthRegister, this->LenSizeTouch);
        return (false);
    }

    if (AbsVector.X > AbsVector.Y)  // left or right
    {
        this->DirectionPlayer = (NoAbsVector.X > 0) ? EDirectionPlayer::Left : EDirectionPlayer::Right;
    }
    else  // up or down
    {
        this->DirectionPlayer = (NoAbsVector.Y > 0) ? EDirectionPlayer::Up : EDirectionPlayer::Down;
    }
    UE_LOG(LogCameraPawn, Display, TEXT("Registered direction player: %s"), *UEnum::GetValueAsString(this->DirectionPlayer));
    return (true);
}

FVector ACameraPawn::TryFindNewPointLocation()
{
    FVector StartLine = this->AIPlayer->GetCapsuleComponent()->GetComponentLocation();
    FVector EndLine = StartLine + this->BaseRotationPlayer[this->DirectionPlayer].RotationPlayer.Vector() * this->DistanceTrace;
    
    FCollisionQueryParams FirstParams(FName(TEXT("param")), false, GetOwner());
    FCollisionObjectQueryParams FirstObjectParams(ECollisionChannel::ECC_WorldStatic);
    
    FHitResult FirstTrace = this->TryGetTrace(StartLine, EndLine, FirstParams, FirstObjectParams);
    if (FirstTrace.IsValidBlockingHit() && FirstTrace.GetActor())
    {
        const auto TempActor = FirstTrace.GetActor();
        UE_LOG(LogCameraPawn, Display, TEXT("Wall is actor: %s"), *TempActor->GetName());
    
        FVector BlockLocationStart = FirstTrace.ImpactPoint;
        BlockLocationStart += this->BaseRotationPlayer[this->DirectionPlayer].TraceLocation;
        FVector BlockLocationEnd = BlockLocationStart + (FRotator(-90.f, 0.f, 0.f)).Vector() * this->DistanceTrace;
    
        FCollisionQueryParams SecondParams(FName(TEXT("param")), false, GetOwner());
        FCollisionObjectQueryParams SecondObjectParams(ECollisionChannel::ECC_OverlapAll_Deprecated);
    
        FHitResult SecondTrace = this->TryGetTrace(BlockLocationStart, BlockLocationEnd, SecondParams, SecondObjectParams);
        if (SecondTrace.IsValidBlockingHit() && SecondTrace.GetActor())
        {
            const auto TempSecondActor = SecondTrace.GetActor();
            UE_LOG(LogCameraPawn, Display, TEXT("Block item is actor: %s"), *TempSecondActor->GetName());
    
            const auto TempPlatform = Cast<AGridPlatformActor>(TempSecondActor);
            if (!TempPlatform) return (FVector::ZeroVector);
    
            const FIntPoint TempPosPlatform = TempPlatform->GetPositionPlatform();
            const FIntPoint TempPosAICharacter = this->AIPlayer->GetPosPlayer();
    
            if (TempPosPlatform == TempPosAICharacter) return (FVector::ZeroVector);
    
            this->AIPlayer->SetNewPosPlayer(TempPosPlatform);
            UE_LOG(LogCameraPawn, Display, TEXT("New Position AI player: %s"), *TempPosPlatform.ToString());
            FVector ResVec = TempSecondActor->GetActorLocation();
    
            return (ResVec);
        }
    }
    return (FVector::ZeroVector);
}

FHitResult ACameraPawn::TryGetTrace(FVector StartPos, FVector EndPos, FCollisionQueryParams Params,
    FCollisionObjectQueryParams ObjectParams)
{
    FHitResult HitResult;

    GetWorld()->LineTraceSingleByObjectType(HitResult, StartPos, EndPos, ObjectParams, Params);
    DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::MakeRandomColor(), false, 5.f, 0, 2.f);
    return (HitResult);
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindTouch(IE_Pressed, this, &ACameraPawn::OnTouchPressed);
        PlayerInputComponent->BindTouch(IE_Released, this, &ACameraPawn::OnTouchReleased);
    }
}

