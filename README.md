# :passenger_ship: PiratePuzzle <a name="main"></a>
## :bookmark_tabs: Table of contents
* [General description of the game](#Description)
* [Devops](#Devops)
* [Technical documentation](#Tech)
  * [Generator Grid](#GenGrid)
  * [Types of platforms](#Types)
* [Other](#Other)

## :page_with_curl: General description of the game <a name="Description"></a>
:moyai: Working title: Pirate Puzzle  
:moyai: Platform: Android  
:moyai: Engine version: Unreal Engine 4.26.2

## :card_index: Devops <a name="Devops"></a>
:open_file_folder: Path: \<Project\>/Devops/\*  
* `clear-project-files.bat` - Clears temporary folders that UE4 creates.
* `format-allfiles.bat` - Manual formatting under .clang-format files (.h .cpp .cs). :bangbang:To work correctly, you need to set the path to clang-format in the environment variable.
* `generate-project-files.bat` - Generates files for the UE4 engine. :bangbang:To work correctly in the script, you need to specify the correct path to the variables: VersionSelector, ProjectName, EnginePath, UBTRelativePath.  
* `install-pre-commit.bat` - Installs bash script in the root of the Hook folder for automatic formatting of clang-format files (.h .cpp .cs)  

## :memo: Technical documentation <a name="Tech"></a>
### :1234: Generator Grid <a name="GenGrid"></a>
:open_file_folder: Path:  \<Project\>/Source/PiratePuzzle/Public(Private)/Game/Grid/GridGenerator.h(.cpp).  
The class `AGridGeneratorActor` is designed to generate the main grid of the game location. It generates classes such as: AGridNeutralPlatform, AGridBarrierPlatform, AGridQuicksandPlatform, AGridTreasurePlatform, AGridWallActor, APirateAICharacter, ASkeletonRunnerCharacter.  
All control of the creation of the grid is carried out through the following functions:  
* `void SpawnPlatform();`
* `void SpawnWall();`
* `void SpawnPirate();`
* `void SpawnSkeletonRunners();`
* `void ClearGrid();`  

The `AGridNeutralPlatform` class uses the following parameters to appear on the grid:  
```
    // Reference for mesh spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "Reference for mesh spawning"))
    TSubclassOf<AGridNeutralPlatform> SpawnNeutralPlatformRef;
    // Size grid
    UPROPERTY(
        EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform", meta = (AllowPrivateAccess = "true", ToolTip = "WidthCount"))
    int32 WidthCount = 6;
    UPROPERTY(
        EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform", meta = (AllowPrivateAccess = "true", ToolTip = "HeightCount"))
    int32 HeightCount = 6;
    // The distance between platforms when spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform",
        meta = (AllowPrivateAccess = "true", ToolTip = "The distance between platforms when spawning"))
    float DistancePlatform = 100.f;
    // Map Point and pointer spawn item on grid
    TMap<FIntPoint, AGridPlatformActor*> MapPlatformsOnGrid;
```

The `AGridBarrierPlatform` class uses the following parameters to appear on the grid:  
```
    // Barrier pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "Barrier pointer ref"))
    TSubclassOf<AGridBarrierPlatform> SpawnBarrierPlatformRef;
    // Barrier point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "Barrier point position for Spawn on platform"))
    TArray<FIntPoint> ArrayPosBarrier;
```

The `AGridQuicksandPlatform` class uses the following parameters to appear on the grid:  
```
    // Quicksand pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Quicksand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Quicksand pointer ref"))
    TSubclassOf<AGridQuicksandPlatform> SpawnQuicksandPlatformRef;
    // Quicksand point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Quicksand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Quicksand point position for Spawn on platform"))
    TArray<FIntPoint> ArrayPosQuicksand;
```  

The `AGridTreasurePlatform` class uses the following parameters to appear on the grid:  
```
    // Treasure pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Treasure",
        meta = (AllowPrivateAccess = "true", ToolTip = "Treasure pointer ref"))
    TSubclassOf<AGridTreasurePlatform> SpawnTreasurePlatformRef;
    // Treasure point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Platform|State Treasure",
        meta = (AllowPrivateAccess = "true", ToolTip = "Treasure point position for Spawn on platform"))
    FIntPoint PosTreasure;
```
  
The `AGridWallActor` class uses the following parameters to appear on the grid:  
```
    // Reference for Wall spawning
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Wall",
        meta = (AllowPrivateAccess = "true", ToolTip = "Reference for Wall spawning"))
    TSubclassOf<AGridWallActor> SpawnWallRef;
    // Size Wall
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Wall",
        meta = (AllowPrivateAccess = "true", ToolTip = "X -> Wall width Y -> Wall height"))
    FVector2D SizeWall = FVector2D::ZeroVector;
    // Array pointer spawn wall on grid
    TArray<AGridWallActor*> WallsGrid;
```
The `APirateAICharacter` class uses the following parameters to appear on the grid:  
```
    // Pirate pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Pirate pointer ref"))
    TSubclassOf<APirateAICharacter> SpawnPirateRef;
    // Pirate point position for Spawn on platform
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Pirate point position for Spawn on platform"))
    FIntPoint PosPirate;
    // Adds along the Z axis when a pirate spawns
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "Adds along the Z axis when a pirate spawns"))
    float AddPiratePosZ = 50.f;
    // The angle of rotation when spawning a pirate
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Pirate",
        meta = (AllowPrivateAccess = "true", ToolTip = "The angle of rotation when spawning a pirate"))
    float PirateRotZ = 0.f;
    // Pointer on pirate
    APirateAICharacter* AIPirate;
```
The `ASkeletonRunnerCharacter` class uses the following parameters to appear on the grid:  
```
    // Pirate pointer ref
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Skeleton Runner pointer ref"))
    TSubclassOf<ASkeletonRunnerCharacter> SpawnSkeletonRunnerRef;
    // Array information for spawn Skeleton Runner
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Array information for spawn Skeleton Runner"))
    TArray<FInfoSpawnSkeletonRunner> ArrayInfoSpawnSkeletonRunners;
    // Adds along the Z axis when a pirate spawns
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawn Skeleton Runner",
        meta = (AllowPrivateAccess = "true", ToolTip = "Adds along the Z axis when a Skeleton Runner spawns"))
    float AddSkeletonRunnerPosZ = 50.f;
    // Pointers on skeleton runners
    TArray<ASkeletonRunnerCharacter*> ArraySkeletonRunners;
```
The `AGridGeneratorActor` class has an override `virtual void OnConstruction(const FTransform& Transform) override;` function that allows you to adjust the necessary grid settings in the editor before the spawn itself in the gameplay.  

### :butter: Types of platforms <a name="Types"></a>
:open_file_folder: Path:  \<Project\>/Source/PiratePuzzle/Public(Private)/Game/Grid/\*.  
The base class for all types of platforms is `AGridPlatformActor`. It defines the basic parameters for all child classes of classes.  
The basic parameters are:  
```
    // Main scene root components
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRoot;
    // Static mesh item
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMeshComponent;
    // Debug position item on grid
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UTextRenderComponent* TextDebugPosition;

    // Current Position on grid generator
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    FIntPoint PlatformPos = FIntPoint(-1, -1);
```
**TODO: Описать дочерние классы**


## 📫 Other <a name="Other"></a>
:bangbang: Attention: If you can improve my trash code then make a pull request.  
:bangbang: Attention: If you still have questions, write to the section Issues.

**:copyright:Authors:**  
  
*[Mark Veligod](https://github.com/markveligod)*  
