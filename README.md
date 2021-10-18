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
:bangbang: Before using scripts, it is important to edit `config.bat`:
* EnginePath  
* UBTRelativePath  
* VersionSelector  
* ProjectRoot  
* ProjectPureName  
  
**P.s. The rest are optional and necessary...**
### misc
* `clean_intermediate_files.bat` - Clears temporary folders that UE4 creates.  
* `format_all_files.bat` - Manual formatting under .clang-format files (.h .cpp .cs).  
* `generate_project_files.bat` - Generates files for the UE4 engine.  
### githooks
* `install_git_hooks.bat` - Installs bash script in the root of the Hook folder for automatic formatting of clang-format files (.h .cpp .cs)  
### cook
* `cook_game.bat` - Builds cookies for the current project.  
### package
* `package_game.bat` - Packs the project for Android.  
### tests
* `create_test_file.bat` - Creates a class template for tests.  


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
List of child classes: AGridNeutralPlatform, AGridBarrierPlatform, AGridQuicksandPlatform and AGridTreasurePlatform.
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

#### :moyai: AGridNeutralPlatform
The `AGridNeutralPlatform` inherited from the `AGridPlatformActor` base class does not have its own parameters at the moment.  

#### :moyai: AGridBarrierPlatform
The `AGridBarrierPlatform` inherited from the `AGridPlatformActor` base class.  
The class has its own collision that restricts the trace check and is a barrier for a pirate on the grid, as well as its own static mesh that is generated randomly by emitting a barrier.
Collision and static mesh parameters:  
```
    // Box collision barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;
    // Base Mesh Component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* BaseMeshBarrier;
```

Static mesh is randomly generated by internal mash and rotation parameters that are contained in arrays:  
```
    // Array Static mesh for barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "A set of meshes for displaying the barrier"))
    TArray<UStaticMesh*> ArrayMeshBarrier;
    // Array Rotation for barrier
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Barrier",
        meta = (AllowPrivateAccess = "true", ToolTip = "A set of turns to display the barrier"))
    TArray<FRotator> ArrayRotationBarrier;
```

Generation occurs both in this `virtual void BeginPlay() override;` function and in this `virtual void OnConstruction(const FTransform& Transform) override;` function one for display in the UE Editor and in the gameplay.  
```
void AGridBarrierPlatform::BeginPlay()
{
    Super::BeginPlay();
    checkf(this->BoxCollision, TEXT("Box collision is nullptr"));
    checkf(this->BaseMeshBarrier, TEXT("Mesh Barrier is nullptr"));
    checkf(this->ArrayMeshBarrier.Num() != 0, TEXT("Array Barrier size is 0"))
    checkf(this->ArrayRotationBarrier.Num() != 0, TEXT("Array Barrier size is 0"))
    
    // Random Set Mesh Barrier
    this->BaseMeshBarrier->SetStaticMesh(this->ArrayMeshBarrier[FMath::RandRange(0, this->ArrayMeshBarrier.Num() - 1)]);
    // Random Rotation Mesh Barrier
    this->BaseMeshBarrier->SetRelativeRotation(this->ArrayRotationBarrier[FMath::RandRange(0, this->ArrayRotationBarrier.Num() - 1)]);
}

void AGridBarrierPlatform::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    // Random Set Mesh Barrier
    this->BaseMeshBarrier->SetStaticMesh(this->ArrayMeshBarrier[FMath::RandRange(0, this->ArrayMeshBarrier.Num() - 1)]);
    // Random Rotation Mesh Barrier
    this->BaseMeshBarrier->SetRelativeRotation(this->ArrayRotationBarrier[FMath::RandRange(0, this->ArrayRotationBarrier.Num() - 1)]);

}
```

#### :moyai: AGridQuicksandPlatform
The `AGridQuicksandPlatform` inherited from the `AGridPlatformActor` base class.  
This class is a dangerous platform on the grid and is able to complete the game with the Game Over state.
The class also has an intersection collision parameter to register the intersection of the main character and the platform.  
```
    // Box collision for register begin overlap with Pirate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;
    
    ...
    
    // Register begin overlap
    UFUNCTION()
    void RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
```
This `AGridQuicksandPlatform` class has a `void DiveAIPirate(float DeltaTime);` function that immerses the character for each frame after registering the intersection by pre-setting the specified parameters:  
```
void AGridQuicksandPlatform::DiveAIPirate(float DeltaTime)
{
    if (this->TimeElyps < this->RateTimeSand)
    {
        float NewAxisZ = FMath::Lerp(this->StartPos, this->EndPos, this->TimeElyps / this->RateTimeSand);
        FVector NewLoc = this->SuicideBomber->GetActorLocation();
        NewLoc.Z = NewAxisZ;
        this->SuicideBomber->SetActorLocation(NewLoc);
        this->TimeElyps += DeltaTime;
        if (this->TimeElyps >= this->RateTimeSand)
        {
            this->TimeElyps = 0.f;
            this->SuicideBomber = nullptr;
            this->bEnableDepth = false;
        }
    }
}

void AGridQuicksandPlatform::RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogGridQuicksandPlatform, Warning, TEXT("Call Register without param OtherActor"));
        return;
    }
    UE_LOG(LogGridQuicksandPlatform, Display, TEXT("Name Platform: %s | Position: %s | Name Actor overlap: %s"), *GetName(),
        *GetPositionPlatform().ToString(), *OtherActor->GetName());
    if (OtherActor->IsA(APirateAICharacter::StaticClass()))
    {
        APirateAICharacter* TempPirate = Cast<APirateAICharacter>(OtherActor);
        TempPirate->SetStateAI(EStateAI::DeathSand);
        TempPirate->GetCharacterMovement()->StopActiveMovement();
        TempPirate->SetActorLocation(this->BoxCollision->GetComponentLocation());
        TempPirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TempPirate->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

        this->SuicideBomber = TempPirate;
        this->StartPos = TempPirate->GetActorLocation().Z;
        this->EndPos = this->StartPos - this->Depth;
        this->bEnableDepth = true;
        GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameOver);
    }
}
```

Class parameters that allow the character to dive:
```
    // Time of immersion in sand
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time of immersion in sand."))
    float RateTimeSand = 1.f;
    // To what depth to immerse on axis Z
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Sand",
        meta = (AllowPrivateAccess = "true", ToolTip = "To what depth to immerse on axis Z."))
    float Depth = 100.f;

    AActor* SuicideBomber;
    bool bEnableDepth = false;
    float TimeElyps = 0.f;
    float StartPos;
    float EndPos;
```

#### :moyai: AGridTreasurePlatform
The `AGridTreasurePlatform` inherited from the `AGridPlatformActor` base class.  
This class registers character intersections to change the state to Game Win.  
Also has a collision for registration via the `void RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);` function and the collision parameter `UBoxComponent* BoxCollision;`:  
```
    // Box collision for register begin overlap with Pirate
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;

    // Register begin overlap
    UFUNCTION()
    void RegisterCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
```



## 📫 Other <a name="Other"></a>
:bangbang: Attention: If you can improve my trash code then make a pull request.  
:bangbang: Attention: If you still have questions, write to the section Issues.

**:copyright:Authors:**  
  
*[Mark Veligod](https://github.com/markveligod)*  
