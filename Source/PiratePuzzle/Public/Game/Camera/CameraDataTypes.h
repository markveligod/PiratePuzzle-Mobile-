#pragma once

#include "CameraDataTypes.generated.h"

UENUM(BlueprintType)
enum class EDirectionPlayer : uint8
{
    None = 0,
    Up,
    Down,
    Right,
    Left
};

USTRUCT(BlueprintType)
struct FInfoTrace
{
    GENERATED_BODY();

    UPROPERTY(EditDefaultsOnly)
    FRotator RotationPlayer;
    UPROPERTY(EditDefaultsOnly)
    FVector TraceLocation;

    FInfoTrace()
    {
        RotationPlayer = FRotator::ZeroRotator;
        TraceLocation = FVector::ZeroVector;
    }

    FInfoTrace(FRotator NewRotationPlayer, FVector NewTraceLocation)
    {
        RotationPlayer = NewRotationPlayer;
        TraceLocation = NewTraceLocation;
    }
    FInfoTrace(const FInfoTrace& Other)
    {
        this->RotationPlayer = Other.RotationPlayer;
        this->TraceLocation = Other.TraceLocation;
    }
};

