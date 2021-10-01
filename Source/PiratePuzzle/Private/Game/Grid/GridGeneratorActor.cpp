// Pirate Puzzle. Contact: markveligod@gmail.com


#include "Game/Grid/GridGeneratorActor.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UtilsLib/BaseUtils.h"
#include "Game/Grid/GridWallActor.h"

// Declaring a static variable for logging
DEFINE_LOG_CATEGORY_STATIC(LogGridGeneratorActor, All, All);

// Sets default values
AGridGeneratorActor::AGridGeneratorActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // Create root component
    this->RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene component");
    SetRootComponent(this->RootScene);

    // Create Static mesh for root component
    this->StaticMeshHandleComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Handle component");
    this->StaticMeshHandleComponent->SetupAttachment(this->RootScene);
}

// Called when the game starts or when spawned
void AGridGeneratorActor::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->RootScene, TEXT("Root Scene component is nullptr"));
    checkf(this->StaticMeshHandleComponent, TEXT("Static Mesh Handle component is nullptr"));
}

void AGridGeneratorActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    this->ClearGrid();
    if (this->SpawnPlatformRef)
        this->SpawnPlatform();
    if (this->SpawnWallRef)
        this->SpawnWall();
}

void AGridGeneratorActor::SpawnPlatform()
{
    for (int32 i = 0; i < this->HeightCount; i++)
    {
        int32 j = 0;
        FVector ZeroWidthPosition = (j == 0) ? GetActorLocation() : BaseUtils::GetZeroPositionOnGrid(i, this->MapPlatformsOnGrid);
        for (j; j < this->WidthCount; j++)
        {
            FIntPoint TempPoint(j, i);
            FVector NewLocation = FVector(ZeroWidthPosition.X + (this->DistancePlatform * j),
                ZeroWidthPosition.Y + (this->DistancePlatform * i), ZeroWidthPosition.Z);
            FTransform SpawnTransform(NewLocation);

            AGridPlatformActor* TempPlatform = GetWorld()->SpawnActorDeferred<AGridPlatformActor>(this->SpawnPlatformRef, SpawnTransform);
            if (!TempPlatform)
            {
                UE_LOG(LogGridGeneratorActor, Error, TEXT("Spawn error on pos: %s"), *TempPoint.ToString());
                continue;
            }
            // Set Point pos on Grid
            TempPlatform->PlatformPos = TempPoint;
            TempPlatform->TextDebugPosition->SetText(FText::FromString(FString::FromInt(TempPoint.X) + " - " + FString::FromInt(TempPoint.Y)));

            // Finish spawn on grid
            TempPlatform->FinishSpawning(SpawnTransform);
            this->MapPlatformsOnGrid.Add(TempPoint, TempPlatform);
        }
    }
}

void AGridGeneratorActor::SpawnWall()
{
    // Find pos width spawn
    FVector SpawnLocationWidth;

    if (this->WidthCount % 2 == 0)
    {
        auto TempFirst = BaseUtils::GetVectorPositionPlatform(FIntPoint(this->WidthCount / 2, 0), this->MapPlatformsOnGrid);
        auto TempSecond = BaseUtils::GetVectorPositionPlatform(FIntPoint((this->WidthCount / 2) - 1, 0), this->MapPlatformsOnGrid);
        SpawnLocationWidth = (TempFirst + TempSecond) / 2;
    }
    else
    {
        SpawnLocationWidth = BaseUtils::GetVectorPositionPlatform(FIntPoint(UKismetMathLibrary::FFloor(this->WidthCount / 2), 0), this->MapPlatformsOnGrid);
    }

    FVector SpawnLocationHeight;
    if (this->HeightCount % 2 == 0)
    {
        auto TempFirst = BaseUtils::GetVectorPositionPlatform(FIntPoint(0, this->HeightCount / 2), this->MapPlatformsOnGrid);
        auto TempSecond = BaseUtils::GetVectorPositionPlatform(FIntPoint(0, (this->HeightCount / 2) - 1), this->MapPlatformsOnGrid);
        SpawnLocationHeight = (TempFirst + TempSecond) / 2;
    }
    else
    {
        SpawnLocationHeight = BaseUtils::GetVectorPositionPlatform(FIntPoint(0, UKismetMathLibrary::FFloor(this->HeightCount / 2)), this->MapPlatformsOnGrid);
    }

    // Calculate Size exta wall
    FVector SizeExtWallWidth;
    SizeExtWallWidth.X = (this->DistancePlatform * this->WidthCount) / 2;
    SizeExtWallWidth.Y = this->SizeWall.X;
    SizeExtWallWidth.Z = this->SizeWall.Y;

    FVector SizeExtWallHeight;
    SizeExtWallHeight.X = (this->DistancePlatform * this->HeightCount) / 2;
    SizeExtWallHeight.Y = this->SizeWall.X;
    SizeExtWallHeight.Z = this->SizeWall.Y;

    // Spawn wall for pos location
    FActorSpawnParameters Params;

    SpawnLocationWidth.Y -= this->DistancePlatform / 2;
    auto FirstWidthWallRef = GetWorld()->SpawnActor<AGridWallActor>(this->SpawnWallRef, SpawnLocationWidth, FRotator::ZeroRotator, Params);
    FirstWidthWallRef->WallBox->SetBoxExtent(SizeExtWallWidth);
    this->WallsGrid.Add(FirstWidthWallRef);

    SpawnLocationHeight.X -= this->DistancePlatform / 2;
    auto FirstHeightWallRef = GetWorld()->SpawnActor<AGridWallActor>(this->SpawnWallRef, SpawnLocationHeight, FRotator(0, -90, 0), Params);
    FirstHeightWallRef->WallBox->SetBoxExtent(SizeExtWallHeight);
    this->WallsGrid.Add(FirstHeightWallRef);

    SpawnLocationWidth.Y += this->DistancePlatform * this->HeightCount;
    auto SecondWidthWallRef = GetWorld()->SpawnActor<AGridWallActor>(this->SpawnWallRef, SpawnLocationWidth, FRotator::ZeroRotator, Params);
    SecondWidthWallRef->WallBox->SetBoxExtent(SizeExtWallWidth);
    this->WallsGrid.Add(SecondWidthWallRef);

    SpawnLocationHeight.X += this->DistancePlatform * this->WidthCount;
    auto SecondHeightWallRef = GetWorld()->SpawnActor<AGridWallActor>(this->SpawnWallRef, SpawnLocationHeight, FRotator(0, -90, 0), Params);
    SecondHeightWallRef->WallBox->SetBoxExtent(SizeExtWallHeight);
    this->WallsGrid.Add(SecondHeightWallRef);
}

void AGridGeneratorActor::ClearGrid()
{
    // Platform
    TArray<AGridPlatformActor*> ArrayPlatform;
    BaseUtils::FillArrayActorOfClass<AGridPlatformActor>(GetWorld(), this->SpawnPlatformRef, ArrayPlatform);
    for (auto TempPlatfrom : ArrayPlatform)
        TempPlatfrom->Destroy();
    this->MapPlatformsOnGrid.Empty();

    // Wall
    TArray<AGridWallActor*> ArrayWall;
    BaseUtils::FillArrayActorOfClass<AGridWallActor>(GetWorld(), this->SpawnWallRef, ArrayWall);
    for (auto TempWall : ArrayWall)
        TempWall->Destroy();
    this->WallsGrid.Empty();
}
