#pragma once

#include "Containers/UnrealString.h"
#include "Game/Grid/GridPlatformActor.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

class BaseUtils
{
public:
    // Return the string from bool
    static FString GetStringFromBool(bool State)
    {
        if (State) return (FString("True"));
        return (FString("False"));
    }

    // Fill the array with a specific class
    template <typename FillClass>
    static void FillArrayActorOfClass(const UObject* WorldContextObject, TSubclassOf<FillClass> ActorClass, TArray<FillClass*>& OutActors)
    {
        TArray<AActor*> TempArray;
        UGameplayStatics::GetAllActorsOfClass(WorldContextObject, FillClass::StaticClass(), TempArray);
        for (AActor* TempActor : TempArray)
        {
            if (TempActor->IsA(FillClass::StaticClass()))
            {
                OutActors.Add(Cast<FillClass>(TempActor));
            }
        }
    }

    // Find the zero position in the map on Grid
    static FVector GetZeroPositionOnGrid(int32 IndexHeight, TMap<FIntPoint, AGridPlatformActor*>& MapPlatformsOnGrid)
    {
        FIntPoint TempPoint(0, IndexHeight);
        if (MapPlatformsOnGrid.Contains(TempPoint)) return (MapPlatformsOnGrid[TempPoint]->GetActorLocation());
        return (FVector::ZeroVector);
    }
    // Get FVector position platform on grid
    static FVector GetVectorPositionPlatform(FIntPoint Pos, TMap<FIntPoint, AGridPlatformActor*>& MapPlatformsOnGrid)
    {
        if (MapPlatformsOnGrid.Contains(Pos)) return (MapPlatformsOnGrid[Pos]->GetActorLocation());
        return (FVector::ZeroVector);
    }
};