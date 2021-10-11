// Pirate Puzzle. Contact: markveligod@gmail.com

#include "Game/AI/Shark/SharkCharacter.h"

#include "Game/AI/Shark/LabelTagActor.h"
#include "Kismet/GameplayStatics.h"
#include "UtilsLib/BaseUtils.h"

// Sets default values
ASharkCharacter::ASharkCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

FVector ASharkCharacter::GetNextPosTag()
{
    FVector TempVector = this->ArrayPosTags[this->PosCountTag];
    this->PosCountTag++;
    if (this->ArrayPosTags.Num() == this->PosCountTag) this->PosCountTag = 0;
    return (TempVector);
}

// Called when the game starts or when spawned
void ASharkCharacter::BeginPlay()
{
    Super::BeginPlay();
    this->ClearTags();
}

void ASharkCharacter::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    this->ClearTags();
    this->ResetArrayPosition();
    if (this->RefLabelTag) this->SpawnTags();
}

void ASharkCharacter::ClearTags()
{
    TArray<ALabelTagActor*> TempArray;
    BaseUtils::FillArrayActorOfClass<ALabelTagActor>(GetWorld(), ALabelTagActor::StaticClass(), TempArray);
    for (const auto TempLabelTag : TempArray)
        TempLabelTag->Destroy();
    this->ArrayTags.Empty();
}

void ASharkCharacter::ResetArrayPosition()
{
    for (int32 i = 0; i < this->ArrayPosTags.Num(); i++)
    {
        if (this->ArrayPosTags[i] == FVector::ZeroVector) this->ArrayPosTags[i] = GetActorLocation();
    }
}

void ASharkCharacter::SpawnTags()
{
    for (auto VectorPos : this->ArrayPosTags)
    {
        auto TempLabelTag = GetWorld()->SpawnActorDeferred<ALabelTagActor>(this->RefLabelTag, FTransform());
        TempLabelTag->SetPositionTag(VectorPos);
        TempLabelTag->FinishSpawning(FTransform());
        this->ArrayTags.Add(TempLabelTag);
    }
}
