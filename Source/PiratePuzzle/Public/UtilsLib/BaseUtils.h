#pragma once

#include "CoreMinimal.h"
#include "Menu/MenuDataTypes.h"
#include "Game/Grid/GridPlatformActor.h"
#include "Kismet/GameplayStatics.h"

class BaseUtils
{
public:
    // Return the string from bool | true == FString("True") | false == FString("False")
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

    // Find the zero position in the map on Grid or Error -> FVector::ZeroVector
    static FVector GetZeroPositionOnGrid(int32 IndexHeight, TMap<FIntPoint, AGridPlatformActor*>& MapPlatformsOnGrid)
    {
        FIntPoint TempPoint(0, IndexHeight);
        if (MapPlatformsOnGrid.Contains(TempPoint)) return (MapPlatformsOnGrid[TempPoint]->GetActorLocation());
        return (FVector::ZeroVector);
    }
    // Get FVector position platform on grid or Error -> FVector::ZeroVector
    static FVector GetVectorPositionPlatform(FIntPoint Pos, TMap<FIntPoint, AGridPlatformActor*>& MapPlatformsOnGrid)
    {
        if (MapPlatformsOnGrid.Contains(Pos)) return (MapPlatformsOnGrid[Pos]->GetActorLocation());
        return (FVector::ZeroVector);
    }

    // template for changing values for a specific data type
    template <typename Data>
    static void SwapData(Data& Data1, Data& Data2)
    {
        Data TempData = Data1;
        Data1 = Data2;
        Data2 = TempData;
    }

    // Determines the status of the level depending on the number of levels
    static ELevelState GetLevelStateFromCoin(const int32 CountCoin)
    {
        if (CountCoin == 1) return (ELevelState::OneGold);
        if (CountCoin == 2) return (ELevelState::TwoGold);
        if (CountCoin == 3) return (ELevelState::ThreeGold);
        return (ELevelState::ZeroGold);
    }

    // Get string minute from second
    static FString GetStringMinute(int32 ValueSec)
    {
        if (ValueSec == 0) return (FString(""));

        FString Result;
        int32 ValueMinute = 0;

        while (ValueSec >= 60)
        {
            ValueMinute++;
            ValueSec -= 60;
        }

        Result += (ValueMinute > 9) ? FString::FromInt(ValueMinute) : FString("0") + FString::FromInt(ValueMinute);
        Result += ":";
        Result += (ValueSec > 9) ? FString::FromInt(ValueSec) : FString("0") + FString::FromInt(ValueSec);

        return (Result);
    }
};
