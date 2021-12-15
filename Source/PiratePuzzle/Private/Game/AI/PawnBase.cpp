// Is the property of WerFEST Software

#include "Game/AI/PawnBase.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Game/GamePlayMode.h"
#include "PPGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogPawnBase, All, All);

// Sets default values
APawnBase::APawnBase()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create RootScene
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
    SetRootComponent(this->RootScene);

    // Create box collision
    this->BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
    this->BoxCollision->SetupAttachment(this->RootScene);
    this->BoxCollision->SetCollisionProfileName("PirateCollision");

    // Create Mesh
    this->BaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
    this->BaseMesh->SetupAttachment(this->RootScene);
    this->BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APawnBase::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root scene is nullptr"));
    checkf(this->BoxCollision, TEXT("Box collision is nullptr"));
    checkf(this->BaseMesh, TEXT("Skeletal Mesh is nullptr"));

    // Cast to current pointer on AGamePlayMode class and Game instance
    this->GameMode = Cast<AGamePlayMode>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameInstance = Cast<UPPGameInstance>(GetWorld()->GetGameInstance());
    checkf(this->GameInstance, TEXT("Game instance is nullptr"));

    // Setting the default value for brains
    this->StateBrain = EStateBrain::Idle;
}

void APawnBase::ChangeStateBrain(const EStateBrain NewState)
{
    if (this->StateBrain == NewState)
    {
        UE_LOG(LogPawnBase, Warning, TEXT("Name pawn: %s | Call function ChangeStateBrain with StateBrain == NewState"), *GetName());
        return;
    }
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | New state brain: %s"), *GetName(), *UEnum::GetValueAsString(NewState));
    this->StateBrain = NewState;
}

void APawnBase::StartMovement(const FVector StartPoint, const FVector EndPoint)
{
    if (this->bMoveToPoint)
    {
        UE_LOG(LogPawnBase, Warning,
            TEXT("Name pawn: %s | Calling a function `StartMovement` with running movement and state bMoveToPoint == true"), *GetName());
        return;
    }
    if (this->StateBrain != EStateBrain::Idle)
    {
        UE_LOG(LogPawnBase, Warning,
            TEXT("Name pawn: %s | Calling a function `StartMovement` with Current state brain: %s != EStateBrain::Idle"), *GetName(),
            *UEnum::GetValueAsString(this->StateBrain));
        return;
    }
    this->StartPointPosition = StartPoint;
    this->EndPointPosition = EndPoint;
    this->TimeMove = this->CalculateTime((EndPoint - StartPoint).Size(), this->DefaultSpeedMove);
    this->TimeMoveEylips = 0.0f;
    this->StateBrain = EStateBrain::Walk;
    this->bMoveToPoint = true;
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | Start Point: %s | End Point: %s | Time Move: %f "), *GetName(),
        *StartPoint.ToString(), *EndPoint.ToString(), this->TimeMove);
}

void APawnBase::StopMovement()
{
    if (GetWorldTimerManager().TimerExists(this->TimerHandleMovement)) GetWorldTimerManager().ClearTimer(this->TimerHandleMovement);
    this->bMoveToPoint = false;
    this->StartPointPosition = FVector::ZeroVector;
    this->EndPointPosition = FVector::ZeroVector;
    this->TimeMoveEylips = 0.0f;
    this->TimeMove = 0.0f;
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | stop movement"), *GetName());
}

void APawnBase::StartMovementTimer(const FVector StartPoint, const FVector EndPoint, const float RateTime)
{
    if (RateTime <= 0.0f)
    {
        this->StartMovement(StartPoint, EndPoint);
        UE_LOG(LogPawnBase, Warning, TEXT("Call function StartMovementTimer with Rate Time == 0.0f"));
        return;
    }
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &APawnBase::StartMovement, StartPoint, EndPoint);
    GetWorldTimerManager().SetTimer(this->TimerHandleMovement, TimerDelegate, RateTime, false);
    if (bEnableDebugInfo)
    {
        DrawDebugSphere(GetWorld(), StartPoint, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false, this->TimeLifeTrace, 0,
            this->ThicknessTrace);
        DrawDebugSphere(GetWorld(), EndPoint, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false, this->TimeLifeTrace, 0,
            this->ThicknessTrace);
        DrawDebugLine(GetWorld(), StartPoint, EndPoint, this->ColorTrace, false, this->TimeLifeTrace, 0, this->ThicknessTrace);
    }

    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | Start MovementTimer: %f"), *GetName(), RateTime);
}

float APawnBase::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
    if (GetWorldTimerManager().TimerExists(this->TimerHandleAnimMontage)) GetWorldTimerManager().ClearTimer(this->TimerHandleAnimMontage);

    UAnimInstance* AnimInstance = this->BaseMesh->GetAnimInstance();
    if (AnimMontage && AnimInstance)
    {
        float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

        if (Duration > 0.f)
        {
            // Start at a given Section.
            if (StartSectionName != NAME_None)
            {
                AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
            }
            return Duration;
        }
    }

    return 0.f;
}

void APawnBase::PlayAnimMontageTimer(UAnimMontage* AnimMontage, float RateTime, float InPlayRate, FName StartSectionName)
{
    if (!AnimMontage)
    {
        UE_LOG(LogPawnBase, Error, TEXT("Name pawn: %s | AnimMontage == nullptr"), *GetName());
        return;
    }

    if (GetWorldTimerManager().TimerExists(this->TimerHandleAnimMontage))
    {
        UE_LOG(LogPawnBase, Error, TEXT("Name pawn: %s | TimerHandleAnimMontage exist"), *GetName());
        return;
    }

    if (RateTime == 0.0f)
    {
        this->PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
        UE_LOG(LogPawnBase, Warning, TEXT("Name pawn: %s | Call function PlayAnimMontageTimer with Rate Time == 0.0f"), *GetName());
        return;
    }

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, "PlayAnimMontage", AnimMontage, InPlayRate, StartSectionName);
    GetWorldTimerManager().SetTimer(this->TimerHandleAnimMontage, TimerDelegate, RateTime, false);

    UE_LOG(
        LogPawnBase, Display, TEXT("Name pawn: %s | Play Anim Montage: %s | Rate time: %f"), *GetName(), *AnimMontage->GetName(), RateTime);
}

FRotator APawnBase::CalculatePawnRotation(FVector StartPoint, FVector EndPoint)
{
    StartPoint.Z = 0.0f;
    EndPoint.Z = 0.0f;
    return ((EndPoint - StartPoint).Rotation());
}

void APawnBase::StartRotate(const FRotator StartRotator, const FRotator EndRotator)
{
    if (this->bRotateToPosition)
    {
        UE_LOG(LogPawnBase, Warning,
            TEXT("Name pawn: %s | Calling a function `StartRotate` with running movement and state bRotateToPosition == true"), *GetName());
        return;
    }
    this->StartRotatePosition = StartRotator;
    this->EndRotatePosition = EndRotator;
    this->TimeRotate = this->CalculateTime((EndRotator.Vector() - StartRotator.Vector()).Size(), this->DefaultSpeedRotate);
    this->TimeRotateEylips = 0.0f;
    this->bRotateToPosition = true;
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | Start Rot: %s | End Rot: %s | Time Rot: %f "), *GetName(), *StartRotator.ToString(),
        *EndRotator.ToString(), this->TimeRotate);
}

void APawnBase::StartRotateTimer(const FRotator StartRotator, const FRotator EndRotator, const float RateTime)
{
    if (RateTime <= 0.0f)
    {
        this->StartRotate(StartRotator, EndRotator);
        UE_LOG(LogPawnBase, Warning, TEXT("Call function StartRotateTimer with Rate Time == 0.0f"));
        return;
    }

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &APawnBase::StartRotate, StartRotator, EndRotator);
    GetWorldTimerManager().SetTimer(this->TimerHandleRotate, TimerDelegate, RateTime, false);
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | Start RotateTimer: %f"), *GetName(), RateTime);
}

void APawnBase::MoveToPoint(const float DeltaTime)
{
    if (this->TimeMoveEylips < this->TimeMove)
    {
        const FVector NewPointLocation =
            FMath::Lerp(this->StartPointPosition, this->EndPointPosition, this->TimeMoveEylips / this->TimeMove);
        SetActorLocation(NewPointLocation);
        this->TimeMoveEylips += DeltaTime;
    }
    else
    {
        SetActorLocation(this->EndPointPosition);
        if (this->StateBrain == EStateBrain::Walk) this->StateBrain = EStateBrain::Idle;
        this->StopMovement();
    }
}

float APawnBase::CalculateTime(float Distance, float Speed)
{
    return (Distance / Speed);
}

void APawnBase::RotateToPosition(float DeltaTime)
{
    if (this->TimeRotateEylips < this->TimeRotate)
    {
        const FRotator NewRotate =
            FMath::Lerp(this->StartRotatePosition, this->EndRotatePosition, this->TimeRotateEylips / this->TimeRotate);
        SetActorRotation(NewRotate);
        this->TimeRotateEylips += DeltaTime;
    }
    else
    {
        this->bRotateToPosition = false;
        this->TimeRotate = 0.0f;
        this->TimeRotateEylips = 0.0f;
        this->StartRotatePosition = FRotator::ZeroRotator;
        this->EndRotatePosition = FRotator::ZeroRotator;
        GetWorldTimerManager().ClearTimer(this->TimerHandleRotate);
    }
}

// Called every frame
void APawnBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (this->bMoveToPoint) this->MoveToPoint(DeltaTime);
    if (this->bRotateToPosition) this->RotateToPosition(DeltaTime);
}
