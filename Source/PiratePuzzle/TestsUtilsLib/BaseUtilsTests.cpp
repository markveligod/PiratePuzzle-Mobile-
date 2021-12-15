// My game copyright

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "PiratePuzzle/TestsUtilsLib/BaseUtilsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "UtilsLib/BaseUtils.h"
#include "Game/Grid/GridNeutralPlatform.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestGetStringFromBool, "PiratePuzzle.UtilsLib.BaseUtils.GetStringFromBool",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestSwapData, "PiratePuzzle.UtilsLib.BaseUtils.SwapData",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestGetLevelStateFromCoin, "PiratePuzzle.UtilsLib.BaseUtils.GetLevelStateFromCoin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestFillArrayActorOfClass, "PiratePuzzle.UtilsLib.BaseUtils.FillArrayActorOfClass",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestGetZeroPositionOnGrid, "PiratePuzzle.UtilsLib.BaseUtils.GetZeroPositionOnGrid",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestGetVectorPositionPlatform, "PiratePuzzle.UtilsLib.BaseUtils.GetVectorPositionPlatform",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FTestGetStringFromBool::RunTest(const FString& Parameters)
{
    AddInfo("FTestGetStringFromBool testing");
    // true
    TestTrueExpr(BaseUtils::GetStringFromBool(true) == FString("True"));
    // false
    TestTrueExpr(BaseUtils::GetStringFromBool(false) == FString("False"));
    return (true);
}

bool FTestSwapData::RunTest(const FString& Parameters)
{
    AddInfo("FTestSwapData testing");
    // integer 3 and 5
    int32 FirstNumber = 3;
    int32 SecondNumber = 5;
    BaseUtils::SwapData(FirstNumber, SecondNumber);

    TestTrueExpr(FirstNumber == 5);
    TestTrueExpr(SecondNumber == 3);

    // String "Hello" and "World"
    FString FirstString("Hello");
    FString SecondString("World");
    BaseUtils::SwapData(FirstString, SecondString);

    TestTrueExpr(FirstString == FString("World"));
    TestTrueExpr(SecondString == FString("Hello"));

    return (true);
}

bool FTestGetLevelStateFromCoin::RunTest(const FString& Parameters)
{
    AddInfo("FTestGetLevelStateFromCoin testing");
    FString InfoStringErr;
    // int32 1 == ELevelState::OneGold
    int32 OneGoldCoin = 1;
    InfoStringErr =
        FString::Printf(TEXT("test value: %i, expected value: %s"), OneGoldCoin, *UEnum::GetValueAsString(ELevelState::OneGold));
    TestEqual(InfoStringErr, BaseUtils::GetLevelStateFromCoin(OneGoldCoin), ELevelState::OneGold);

    // int32 2 == ELevelState::TwoGold
    int32 TwoGoldCoin = 2;
    InfoStringErr =
        FString::Printf(TEXT("test value: %i, expected value: %s"), TwoGoldCoin, *UEnum::GetValueAsString(ELevelState::TwoGold));
    TestEqual(InfoStringErr, BaseUtils::GetLevelStateFromCoin(TwoGoldCoin), ELevelState::TwoGold);

    // int32 3 == ELevelState::ThreeGold
    int32 ThreeGoldCoin = 3;
    InfoStringErr =
        FString::Printf(TEXT("test value: %i, expected value: %s"), ThreeGoldCoin, *UEnum::GetValueAsString(ELevelState::ThreeGold));
    TestEqual(InfoStringErr, BaseUtils::GetLevelStateFromCoin(ThreeGoldCoin), ELevelState::ThreeGold);

    // int32 0 == ELevelState::ThreeGold
    int32 ZeroGoldCoin = 0;
    InfoStringErr =
        FString::Printf(TEXT("test value: %i, expected value: %s"), ZeroGoldCoin, *UEnum::GetValueAsString(ELevelState::ZeroGold));
    TestEqual(InfoStringErr, BaseUtils::GetLevelStateFromCoin(ZeroGoldCoin), ELevelState::ZeroGold);

    return (true);
}

bool FTestFillArrayActorOfClass::RunTest(const FString& Parameters)
{
    AddInfo("FTestFillArrayActorOfClass non-testing");

    return (true);
}

bool FTestGetZeroPositionOnGrid::RunTest(const FString& Parameters)
{
    AddInfo("FTestGetZeroPositionOnGrid testing");
    FString InfoStringErr;
    TMap<FIntPoint, AGridPlatformActor*> MapPlatformsOnGrid;

    // 10 objects with a distance of 100.0f on all axes
    float GeneralLocation = 100.0f;
    int32 CountObj = 10;
    for (int32 i = 0; i < CountObj; i++)
    {
        AGridPlatformActor* TempPlatfrom = NewObject<AGridPlatformActor>();
        TestNotNull(TEXT("TempPlatform is nullptr"), TempPlatfrom);
        TempPlatfrom->SetActorLocation(FVector(i * GeneralLocation));
        MapPlatformsOnGrid.Add(FIntPoint(0, i), TempPlatfrom);
    }

    // Testing Height 1
    int32 TestValueOne = 1;
    FVector ExpectedValueOne(100.0f);
    InfoStringErr = FString::Printf(TEXT("Test One | test value: %i, expected value: %s"), TestValueOne, *ExpectedValueOne.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetZeroPositionOnGrid(TestValueOne, MapPlatformsOnGrid), ExpectedValueOne);

    // Testing Height 5
    int32 TestValueFive = 5;
    FVector ExpectedValueFive(500.0f);
    InfoStringErr = FString::Printf(TEXT("Test Two | test value: %i, expected value: %s"), TestValueFive, *ExpectedValueFive.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetZeroPositionOnGrid(TestValueFive, MapPlatformsOnGrid), ExpectedValueFive);

    // Testing Height 10
    int32 TestValueTen = 10;
    FVector ExpectedValueTen(FVector::ZeroVector);
    InfoStringErr = FString::Printf(TEXT("Test Three | test value: %i, expected value: %s"), TestValueTen, *ExpectedValueTen.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetZeroPositionOnGrid(TestValueTen, MapPlatformsOnGrid), ExpectedValueTen);

    return (true);
}

bool FTestGetVectorPositionPlatform::RunTest(const FString& Parameters)
{
    AddInfo("FTestGetVectorPositionPlatform testing");
    FString InfoStringErr;
    TMap<FIntPoint, AGridPlatformActor*> MapPlatformsOnGrid;

    // 10 objects with a distance of 100.0f on all axes
    float GeneralLocation = 100.0f;
    int32 CountObj = 10;
    for (int32 i = 0; i < CountObj; i++)
    {
        AGridPlatformActor* TempPlatfrom = NewObject<AGridPlatformActor>();
        TestNotNull(TEXT("TempPlatform is nullptr"), TempPlatfrom);
        TempPlatfrom->SetActorLocation(FVector(i * GeneralLocation));
        MapPlatformsOnGrid.Add(FIntPoint(i, i), TempPlatfrom);
    }

    // Testing FIntPoint(0, 0);
    FIntPoint TestValueZeroZero(0, 0);
    FVector ExpectedValueZeroVector(FVector::ZeroVector);
    InfoStringErr = FString::Printf(
        TEXT("Test One | test value: %s, expected value: %s"), *TestValueZeroZero.ToString(), *ExpectedValueZeroVector.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetVectorPositionPlatform(TestValueZeroZero, MapPlatformsOnGrid), ExpectedValueZeroVector);

    // Testing FIntPoint(0, 1);
    FIntPoint TestValueZeroOne(0, 1);
    InfoStringErr = FString::Printf(
        TEXT("Test Two | test value: %s, expected value: %s"), *TestValueZeroOne.ToString(), *ExpectedValueZeroVector.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetVectorPositionPlatform(TestValueZeroOne, MapPlatformsOnGrid), ExpectedValueZeroVector);

    // Testing FIntPoint(5, 5);
    FIntPoint TestValueFiveFive(5, 5);
    FVector ExpectedValueFiveVector(500.0f);
    InfoStringErr = FString::Printf(
        TEXT("Test Three | test value: %s, expected value: %s"), *TestValueFiveFive.ToString(), *ExpectedValueFiveVector.ToString());
    TestEqual(InfoStringErr, BaseUtils::GetVectorPositionPlatform(TestValueFiveFive, MapPlatformsOnGrid), ExpectedValueFiveVector);

    return (true);
}

#endif
