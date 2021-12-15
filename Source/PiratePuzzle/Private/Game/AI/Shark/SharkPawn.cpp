// Is the property of WerFEST Software

#include "Game/AI/Shark/SharkPawn.h"
#include "Game/AI/Shark/LabelTagActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UtilsLib/BaseUtils.h"

void ASharkPawn::StopMovement()
{
    Super::StopMovement();
    this->GoToNextPosTag();
}

// Called when the game starts or when spawned
void ASharkPawn::BeginPlay()
{
    Super::BeginPlay();
    this->ClearTags();
    this->GoToNextPosTag();
}

void ASharkPawn::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    this->ClearTags();
    this->ResetArrayPosition();
    if (this->RefLabelTag) this->SpawnTags();
}

void ASharkPawn::ClearTags()
{
    TArray<ALabelTagActor*> TempArray;
    BaseUtils::FillArrayActorOfClass<ALabelTagActor>(GetWorld(), ALabelTagActor::StaticClass(), TempArray);
    for (const auto TempLabelTag : TempArray)
        TempLabelTag->Destroy();
    this->ArrayTags.Empty();
}

void ASharkPawn::ResetArrayPosition()
{
    for (int32 i = 0; i < this->ArrayPosTags.Num(); i++)
    {
        if (this->ArrayPosTags[i] == FVector::ZeroVector) this->ArrayPosTags[i] = GetActorLocation();
    }
}

void ASharkPawn::SpawnTags()
{
    for (auto VectorPos : this->ArrayPosTags)
    {
        auto TempLabelTag = GetWorld()->SpawnActorDeferred<ALabelTagActor>(this->RefLabelTag, FTransform());
        TempLabelTag->SetPositionTag(VectorPos);
        TempLabelTag->FinishSpawning(FTransform());
        this->ArrayTags.Add(TempLabelTag);
    }
}

FVector ASharkPawn::GetNextPosTag()
{
    const FVector TempVector = this->ArrayPosTags[this->PosCountTag];
    this->PosCountTag++;
    if (this->ArrayPosTags.Num() == this->PosCountTag) this->PosCountTag = 0;
    return (TempVector);
}

void ASharkPawn::GoToNextPosTag()
{
    const FVector NextTag = this->GetNextPosTag();
    const float RateTime = UKismetMathLibrary::RandomFloatInRange(this->RangeMove.Min, this->RangeMove.Max);
    StartRotateTimer(GetActorRotation(), CalculatePawnRotation(GetActorLocation(), NextTag), RateTime);
    StartMovementTimer(GetActorLocation(), NextTag, RateTime);
}
