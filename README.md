# :passenger_ship: PiratePuzzle <a name="main"></a>
## :bookmark_tabs: Table of contents
* [General description of the game](#Description)
* [Devops](#Devops)
* [Technical documentation](#Tech)
  * [Generator Grid](#GenGrid)
  * [Types of platforms](#Types)
  * [Camera](#Camera)
  * [AI](#AI)
* [Other](#Other)

## :page_with_curl: General description of the game <a name="Description"></a>
:moyai: Working title: Pirate Puzzle  
:moyai: Platform: Android  
:moyai: Engine version: Unreal Engine 4.26.2

[UP Table of contents](#main)  

## :card_index: Devops <a name="Devops"></a>
:open_file_folder: Path: \<Project\>/Devops/\*  
:bangbang: Before using scripts, it is important to edit `config.bat`:
* EnginePath  
* UBTRelativePath  
* VersionSelector  
* ProjectRoot  
* ProjectPureName  
  
**P.s. The rest are optional and necessary...**  

:bangbang: For clang-format to work correctly, you need to set the full path to the environment variables.  
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

[UP Table of contents](#main)  

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

[UP Table of contents](#main)  

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
  
[UP Table of contents](#main)  

### :movie_camera: Camera <a name="Camera"></a>
:open_file_folder: Path:  \<Project\>/Source/PiratePuzzle/Public(Private)/Game/Camera/CameraPawn.h(.cpp).  
This class `ACameraPawn` contains the basic logic for controlling the pirate character on the exposed grid.  
To set the necessary parameters, the following components are exposed:  
```
    // root Scene component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USceneComponent* SceneRootComponent;
    // Sphere pre-made
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* SphereComponent;
    // Camera player
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* CameraComponent;
    // Distance for camera
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArmComponent;

```
The class `ACameraPawn` also determines the management of the `APirateAICharacter` class and performs registration on the fingers. This `PlayerInputComponent` bind functions for tracking the IE_Pressed event and IE_Released through the method `virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;` :  
```
    // Size register touch control
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Touch",
        meta = (AllowPrivateAccess = "true", ToolTip = "Size register touch control"))
    float LenSizeTouch = 100.f;
    // Distance general trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Touch",
        meta = (AllowPrivateAccess = "true", ToolTip = "Distance general trace"))
    float DistanceTrace = 1000.f;
    // starting position of the finger
    FVector2D StartTouch;
    // end position of the finger
    FVector2D EndTouch;
    
    ...
    
    // Called to bind functionality to input
    void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
    {
        Super::SetupPlayerInputComponent(PlayerInputComponent);
        if (PlayerInputComponent)
        {
            PlayerInputComponent->BindTouch(IE_Pressed, this, &ACameraPawn::OnTouchPressed);
            PlayerInputComponent->BindTouch(IE_Released, this, &ACameraPawn::OnTouchReleased);
        }
    }
```
  
After recording the starting and ending positions by Pressed and Released with finger, the direction is calculated through the method `bool UpdateDirectionForPlayer();` and the position is determined through the method `FIntPoint TryFindNewPointLocation();` on the next platform for the `APirateAICharacter` class.  
```
bool ACameraPawn::UpdateDirectionForPlayer()
{
    const auto NoAbsVector = FVector2D(this->StartTouch.X - this->EndTouch.X, this->StartTouch.Y - this->EndTouch.Y);
    const auto AbsVector = FVector2D(FMath::Abs(this->StartTouch.X - this->EndTouch.X), FMath::Abs(this->StartTouch.Y - this->EndTouch.Y));
    const auto LengthRegister =
        UKismetMathLibrary::VSize2D(FVector2D(this->EndTouch.X - this->StartTouch.X, this->EndTouch.Y - this->StartTouch.Y));

    if (LengthRegister < this->LenSizeTouch)
    {
        UE_LOG(LogCameraPawn, Error, TEXT("Length Register: %f less define len register:  %f"), LengthRegister, this->LenSizeTouch);
        return (false);
    }

    if (AbsVector.X > AbsVector.Y)  // left or right
    {
        this->DirectionPlayer = (NoAbsVector.X > 0) ? EDirectionPlayer::Left : EDirectionPlayer::Right;
    }
    else  // up or down
    {
        this->DirectionPlayer = (NoAbsVector.Y > 0) ? EDirectionPlayer::Up : EDirectionPlayer::Down;
    }
    UE_LOG(LogCameraPawn, Display, TEXT("Registered direction player: %s"), *UEnum::GetValueAsString(this->DirectionPlayer));
    return (true);
}

FIntPoint ACameraPawn::TryFindNewPointLocation()
{
    FVector StartLine = this->AIPlayer->GetCapsuleComponent()->GetComponentLocation();
    FVector EndLine = StartLine + this->BaseRotationPlayer[this->DirectionPlayer].RotationPlayer.Vector() * this->DistanceTrace;

    FCollisionQueryParams FirstParams(FName(TEXT("param")), false, GetOwner());
    FCollisionObjectQueryParams FirstObjectParams(ECollisionChannel::ECC_WorldStatic);

    FHitResult FirstTrace = this->TryGetTrace(StartLine, EndLine, FirstParams, FirstObjectParams);
    if (FirstTrace.IsValidBlockingHit() && FirstTrace.GetActor())
    {
        const auto TempActor = FirstTrace.GetActor();
        UE_LOG(LogCameraPawn, Display, TEXT("Wall is actor: %s"), *TempActor->GetName());

        FVector BlockLocationStart = FirstTrace.ImpactPoint;
        BlockLocationStart += this->BaseRotationPlayer[this->DirectionPlayer].TraceLocation;
        FVector BlockLocationEnd = BlockLocationStart + (FRotator(-90.f, 0.f, 0.f)).Vector() * this->DistanceTrace;
        // Draw debug trace
        if (this->bEnableDebugTrace)
        {
            DrawDebugLine(GetWorld(), StartLine, BlockLocationStart, this->ColorTrace, false, this->TimeLifeTrace, 0, this->ThicknessTrace);
            DrawDebugSphere(GetWorld(), BlockLocationStart, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false,
                this->TimeLifeTrace, 0, this->ThicknessTrace);
        }
        FCollisionQueryParams SecondParams(FName(TEXT("param")), false, GetOwner());
        FCollisionObjectQueryParams SecondObjectParams(ECollisionChannel::ECC_OverlapAll_Deprecated);

        FHitResult SecondTrace = this->TryGetTrace(BlockLocationStart, BlockLocationEnd, SecondParams, SecondObjectParams);
        if (SecondTrace.IsValidBlockingHit() && SecondTrace.GetActor())
        {
            const auto TempSecondActor = SecondTrace.GetActor();
            UE_LOG(LogCameraPawn, Display, TEXT("Block item is actor: %s"), *TempSecondActor->GetName());
            // Draw debug trace
            if (this->bEnableDebugTrace)
            {
                DrawDebugLine(GetWorld(), BlockLocationStart, SecondTrace.ImpactPoint, this->ColorTrace, false, this->TimeLifeTrace, 0,
                    this->ThicknessTrace);
                DrawDebugSphere(GetWorld(), SecondTrace.ImpactPoint, this->RadiusSphere, this->SegmentsSphere, this->ColorTrace, false,
                    this->TimeLifeTrace, 0, this->ThicknessTrace);
            }

            const auto TempPlatform = Cast<AGridPlatformActor>(TempSecondActor);
            if (!TempPlatform) return (FIntPoint(-1, -1));

            const FIntPoint TempPosPlatform = TempPlatform->GetPositionPlatform();
            const FIntPoint TempPosAICharacter = this->AIPlayer->GetPosPlayer();

            if (TempPosPlatform == TempPosAICharacter) return (FIntPoint(-1, -1));

            FIntPoint BeforePos = this->AIPlayer->GetPosPlayer();
            if (BeforePos != FIntPoint(-1, -1)) this->AIPlayer->AddBeforePos(BeforePos);
            UE_LOG(LogCameraPawn, Display, TEXT("New Position AI player: %s"), *TempPosPlatform.ToString());
            return (TempPosPlatform);
        }
    }
    return (FIntPoint(-1, -1));
}
```
  
The class also implements debug information on the installation direction of determining the next position of the platform through the parameters:  
```
    /*
     * Debug line trace from pirate
     */
    // Enable debug rendering of pirate movement lines.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Enable debug rendering of pirate movement lines."))
    bool bEnableDebugTrace = true;
    // Color debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Color debug trace.", EditCondition = "bEnableDebugTrace"))
    FColor ColorTrace = FColor::Red;
    // Life time debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Life time debug trace.", EditCondition = "bEnableDebugTrace"))
    float TimeLifeTrace = 5.f;
    // Thickness debug trace
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Thickness debug trace.", EditCondition = "bEnableDebugTrace"))
    float ThicknessTrace = 5.f;
    // Radius debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Radius debug sphere.", EditCondition = "bEnableDebugTrace"))
    float RadiusSphere = 5.f;
    // Segments debug sphere
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Debug Trace",
        meta = (AllowPrivateAccess = "true", ToolTip = "Segments debug sphere.", EditCondition = "bEnableDebugTrace"))
    int32 SegmentsSphere = 5.f;

```
  
[UP Table of contents](#main)  

### :robot: AI <a name="AI"></a>
:bangbang: The AI Module is completely cut out in the project for more optimized work on a smartphone.  
:open_file_folder: Path:  \<Project\>/Source/PiratePuzzle/Public(Private)/Game/AI/\*  
The main class that is the parent for all child is `APawnBase` class. Classes such as are inherited from it: APiratePawn, ASharkPawn, ASkeletonCannonPawn and ASkeletonRunnerPawn.  
The main purpose of the class is to provide basic functionality for moving on the Pawn grid as well as storing any states.  
The `APawnBase` class contains the following basic components:
```
    // Scene root component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USceneComponent* RootScene;
    // A collision for a pawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UBoxComponent* BoxCollision;
    // Skeletal mesh component
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    USkeletalMeshComponent* BaseMesh;
```

Public functions for getting pointers to components:  
```
    // Getting pawn skeletal mesh component
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting pawn skeletal mesh component"))
    USkeletalMeshComponent* GetMesh() const { return (this->BaseMesh); }

    // Getting collision component
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Getting collision component"))
    UBoxComponent* GetCollision() const { return (this->BoxCollision); }
```

The first global implementation of the class is the Pawn movement logic. The calculation using linear interpolation is used as a movement.  
The following parameters are used for implementation:  
```
    // Movement speed pawn cm/sec
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = true, ToolTip = "Movement speed pawn cm/sec", ClampMin = "100.0", ClampMax = "1500.0"))
    float DefaultSpeedMove = 1000.0f;

    // The zero state of the move lerp
    bool bMoveToPoint = false;
    // Vector of the starting point
    FVector StartPointPosition = FVector::ZeroVector;
    // Vector of the final position for movement
    FVector EndPointPosition = FVector::ZeroVector;
    // Intermediate value for time travel
    float TimeMoveEylips = 0.0f;
    // Time for move
    float TimeMove = 0.0f;
```

To provide flexibility in the use of movement for control from other classes, functions are used: `virtual void StartMovement(FVector StartPoint, FVector EndPoint);` and `virtual void StopMovement();`  
```
    // Start Movement Pawn
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Start Movement Pawn"))
    virtual void StartMovement(FVector StartPoint, FVector EndPoint);

    // Stop Movement Pawn
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Stop Movement Pawn"))
    virtual void StopMovement();
    
    ...
    
void APawnBase::StartMovement(FVector StartPoint, FVector EndPoint)
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

```

For higher efficiency, the function `virtual void StartMovement(FVector StartPoint, FVector EndPoint);` has a wrapper in the form of a function `void StartMovementTimer(FVector StartPoint, FVector EndPoint, float RateTime);` that allows you to call on a timer.  
```
    // The function of starting the timer for the next move
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "The function of starting the timer for the next move"))
    void StartMovementTimer(FVector StartPoint, FVector EndPoint, float RateTime);
    
    ...
    
void APawnBase::StartMovementTimer(FVector StartPoint, FVector EndPoint, float RateTime)
{
    if (RateTime == 0.0f)
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
}
```

The second global thing of the class `APawnBase` is to rotate Pawn in any direction using the same linear interpolation.  
The following parameters are used to implement rotation:  
```
    // Speed Rotate pawn
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = true, ToolTip = "Speed Rotate pawn", ClampMin = "1.0", ClampMax = "10.0"))
    float DefaultSpeedRotate = 5.0f;

    // The zero state of the rotation lerp
    bool bRotateToPosition = false;
    // Vector of the starting point
    FRotator StartRotatePosition = FRotator::ZeroRotator;
    // Vector of the final position for movement
    FRotator EndRotatePosition = FRotator::ZeroRotator;
    // Intermediate value for Rotate travel
    float TimeRotateEylips = 0.0f;
    // Time for Rotate
    float TimeRotate = 0.0f;
```

The implementation is a protected function: `void StartRotate(FRotator StartRotator, FRotator EndRotator);` and as a wrapper `void StartRotateTimer(FRotator StartRotator, FRotator EndRotator, float RateTime);`
```
    // Starting rotation at certain points
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Starting rotation at certain points"))
    void StartRotate(FRotator StartRotator, FRotator EndRotator);

    // Starting rotation at timer
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Starting rotation at timer"))
    void StartRotateTimer(FRotator StartRotator, FRotator EndRotator, float RateTime);

    ...
    
void APawnBase::StartRotate(FRotator StartRotator, FRotator EndRotator)
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

void APawnBase::StartRotateTimer(FRotator StartRotator, FRotator EndRotator, float RateTime)
{
    if (RateTime == 0.0f)
    {
        this->StartRotate(StartRotator, EndRotator);
        UE_LOG(LogPawnBase, Warning, TEXT("Call function StartRotateTimer with Rate Time == 0.0f"));
        return;
    }

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUObject(this, &APawnBase::StartRotate, StartRotator, EndRotator);
    GetWorldTimerManager().SetTimer(this->TimerHandleRotate, TimerDelegate, RateTime, false);
}

```

Global movement and global rotation are both implemented on the call of each frame by controlling two Boolean states: `bool bMoveToPoint = false;` and `bool bRotateToPosition = false;`  
```
// Called every frame
void APawnBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (this->bMoveToPoint) this->MoveToPoint(DeltaTime);
    if (this->bRotateToPosition) this->RotateToPosition(DeltaTime);
}
```

Also, the class `APawnBase` stores the state of the brains for Pawn: `EStateBrain StateBrain = EStateBrain::None;`. It also has a public function `virtual void ChangeStateBrain(EStateBrain NewState);`.  
It looks like this:
```
// A special state for a skeleton with a cannon
UENUM(BlueprintType)
enum class EStateBrain : uint8
{
    None = 0,
    Idle,
    Walk,
    Sand,
    FellCannon,
    FellSkeleton,
    Lose,
    Win,
    Attack,
    Fire,
    Reload
};

...

// Pawn Behavior States
EStateBrain StateBrain = EStateBrain::None;

...

    // Change pawn behavior
    UFUNCTION(BlueprintCallable, Category = "APawnBase", meta = (ToolTip = "Change pawn behavior"))
    virtual void ChangeStateBrain(EStateBrain NewState);
    
    ...
    
void APawnBase::ChangeStateBrain(EStateBrain NewState)
{
    if (this->StateBrain == NewState)
    {
        UE_LOG(LogPawnBase, Warning, TEXT("Name pawn: %s | Call function ChangeStateBrain with StateBrain == NewState"), *GetName());
        return;
    }
    UE_LOG(LogPawnBase, Display, TEXT("Name pawn: %s | New state brain: %s"), *GetName(), *UEnum::GetValueAsString(NewState));
    this->StateBrain = NewState;
}

```

#### ASharkPawn
The simplest heir class is `ASharkPawn`. This class simulates the primitive movement of a shark at the level.  
The main action is to collect points in an array along which the shark will move using the functionality of the parent `APawnBase` class.  
The `ALabelTagActor` class is used as a marker at the level of the movement point.  
This is implemented through the following parameters:  
```
    // A reference for creating a pointer to the location of the next location.
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "A reference for creating a pointer to the location of the next location."))
    TSubclassOf<ALabelTagActor> RefLabelTag;
    // Array of positions on which the shark will move
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "Array of positions on which the shark will move."))
    TArray<FVector> ArrayPosTags;
    // Time pause movement to the next point
    UPROPERTY(EditInstanceOnly, Category = "Settings Move",
        meta = (AllowPrivateAccess = "true", ToolTip = "Time pause movement to the next point."))
    FIntervalFloat RangeMove{2.0f, 5.0f};

    // Current count position in Array
    int32 PosCountTag = 0;

    // Array tags
    TArray<ALabelTagActor*> ArrayTags;
```

Movement is carried out by expanding the functionality `virtual void StopMovement() override;` thereby there is a fixation on moving around the function `FVector GetNextPosTag();`  
```
void ASharkPawn::StopMovement()
{
    Super::StopMovement();
    this->GoToNextPosTag();
}

...

void ASharkPawn::GoToNextPosTag()
{
    const FVector NextTag = this->GetNextPosTag();
    const float RateTime = UKismetMathLibrary::RandomFloatInRange(this->RangeMove.Min, this->RangeMove.Max);
    StartRotateTimer(GetActorRotation(), CalculatePawnRotation(GetActorLocation(), NextTag), RateTime);
    StartMovementTimer(GetActorLocation(), NextTag, RateTime);
}
```

#### APiratePawn
The same class that is controlled by the `ACameraPawn` class and it is from it that the rays are calculated to calculate the movement along the grid.  
The class stores an array of those grid points that it has already passed. These parameters look like this:  
```
    // Position Player on grid
    FIntPoint PiratePointGrid = FIntPoint(-1, -1);
    // Array Position
    TArray<FIntPoint> ArrayPointPosition;
```
Also public functions for more efficient management of the array of points:  
```
    // Setting New point position on grid
    void SetNewPoint(const FIntPoint& NewPoint) { this->PiratePointGrid = NewPoint; }

    // Getting the previous position on the grid or return Error (FIntPoint(-1, -1))
    FIntPoint GetPreviousPoint();
    
    ...
    
FIntPoint APiratePawn::GetPreviousPoint()
{
    if (this->ArrayPointPosition.Num() > 0)
    {
        FIntPoint TempPoint(this->ArrayPointPosition.Last());
        this->ArrayPointPosition.RemoveAt(this->ArrayPointPosition.Num() - 1);
        UE_LOG(LogPiratePawn, Display, TEXT("Name pawn: %s | Return last point: %s"), *GetName(), *TempPoint.ToString());
        return (TempPoint);
    }
    return (FIntPoint(-1, -1));
}

...

    // Getting current point position on grid
    FIntPoint GetPointPosition() const { return (this->PiratePointGrid); }

    // Adding a position to an ArrayPointPosition
    void AddPreviousPointPosition(const FIntPoint& NewPoint) { this->ArrayPointPosition.Add(NewPoint); }
```

#### ASkeletonRunnerPawn
Similar to the class `APiratePawn` there is a class similar in behavior `ASkeletonRunnerPawn` that moves according to the same rules as the main character. This class `ASkeletonRunnerPawn` of enemy is dangerous for the main player and can pre-finish the game.  
Of the components , the class `ASkeletonRunnerPawn` has:  
```
    // Static mash component for displaying the attached stone
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
    UStaticMeshComponent* StoneMesh;
    // The name of the socket on the skeleton grid to attach to
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Runner",
        meta = (AllowPrivateAccess = true, ToolTip = "The name of the socket on the skeleton grid to attach to."))
    FName StoneNameSocket = "RockSocket";
```
This component only extends the parent class `APawnBase` and adds a visual static mash of the stone.  
The prince of building movement is to switch the state by timer. This is done through an array of values and call public functions at the next, current or previous coordinates of the array:  
```
    // State is Revers from ArrayPointLocation
    bool bIsReverseArray = false;
    // Count for define next location
    int32 CountLoc = 0;
    // Array point position for road route
    TArray<FIntPoint> ArrayPointLocation;
    // Current position on Grid
    FIntPoint SkeletonPosition;
    
    ...
    
    // Go Go Go runner to Next point
    UFUNCTION(BlueprintCallable, Category = "ASkeletonRunnerPawn", meta = (ToolTip = "Go Go Go runner to Next point."))
    void GoToNextPoint();

    // Go Go Go runner to Current point
    void GoToCurrentPoint();

    // Go Go Go runner to Prev point
    void GoToPrevPoint();
```

To register an intersection event, the `UBoxComponent* BoxCollision;` on `OnComponentBeginOverlap` delegate and the callback function `void OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);`:
```
void ASkeletonRunnerPawn::OnRegisterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
    {
        UE_LOG(LogSkeletonRunnerPawn, Warning, TEXT("Other actor is nullptr"));
        return;
    }
    UE_LOG(LogSkeletonRunnerPawn, Display, TEXT("Skeleton runner: %s | Overlap Actor: %s"), *GetName(), *OtherActor->GetName());

    if (OtherActor->IsA(APiratePawn::StaticClass()))
    {
        // Dead Pirate
        APiratePawn* TempPirate = Cast<APiratePawn>(OtherActor);
        TempPirate->StopMovement();
        TempPirate->ChangeStateBrain(EStateBrain::FellSkeleton);
        const FVector DirectionToSkeleton = (GetActorLocation() - TempPirate->GetActorLocation()).GetSafeNormal();
        TempPirate->SetActorRotation(DirectionToSkeleton.Rotation());

        // Skeleton runner attack
        ChangeStateBrain(EStateBrain::Attack);
        StopMovement();
        const FVector DirectionToPirate = (TempPirate->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorRotation(DirectionToPirate.Rotation());

        // Game Over
        GetGamePlayMode()->OnChangeGameStateTimer(EGameState::GameOver, this->RateGameOverTimer);
    }
}
```

#### ASkeletonCannonPawn
The last descendant from the parent `APawnBase` class is the `ASkeletonCannonPawn` class. It defines the behavior of a skeleton firing a cannon with solid shells from the `ABulletActor` class.
The basic principle of the shot is realized through `USpawnBulletAnimNotify` which is registered in the animation `UAnimSequence* AnimShoot = nullptr;`.
Registration is carried out in the protected function `virtual void BeginPlay() override;` :
```
// Called when the game starts or when spawned
void ASkeletonCannonPawn::BeginPlay()
{
    Super::BeginPlay();

    ...

    // Caption for the animated shot notification
    const auto ListNotifies = this->AnimShoot->Notifies;
    for (auto TempClassNotify : ListNotifies)
    {
        auto CastNotify = Cast<USpawnBulletAnimNotify>(TempClassNotify.Notify);
        if (CastNotify)
        {
            UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Name skeleton: %s | Name notify to cast: %s"), *GetName(), *CastNotify->GetName());
            CastNotify->OnSpawnBulletAnimNotify.AddUObject(this, &ASkeletonCannonPawn::SpawnBulletFromNotify);
        }
    }
}
```
  
The state of reloading and firing is changed by timer depending on the animation being played in the function `void SwapAnimState();` :
```
void ASkeletonCannonPawn::SwapAnimState()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerAnimFR);

    ChangeStateBrain((GetStateBrain() == EStateBrain::Fire) ? EStateBrain::Reload : EStateBrain::Fire);
    UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Name skeleton: %s | Swap state to %s"), *GetName(),
        *UEnum::GetValueAsString(GetStateBrain()));

    const float TempRate = (GetStateBrain() == EStateBrain::Fire) ? this->TimeRateFire : this->TimeRateReload;
    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, TempRate, false);
}
```
  
Spawn `ABulletActor` of the class on Notify is carried out in the function `void SpawnBulletFromNotify();` :
```
void ASkeletonCannonPawn::SpawnBulletFromNotify()
{
    FTransform TempTransform;
    TempTransform.SetLocation(this->SceneSpawnBullet->GetComponentLocation());
    TempTransform.SetRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));
    TempTransform.SetScale3D(FVector(1.0f));

    ABulletActor* TempBullet = GetWorld()->SpawnActorDeferred<ABulletActor>(this->RefBulletClass, TempTransform);
    if (!TempBullet)
    {
        UE_LOG(LogSkeletonCannonPawn, Error, TEXT("Skeleton cannon: %s | spawn bullet is nullptr"), *GetName());
        return;
    }

    TempBullet->SetActorLocation(this->SceneSpawnBullet->GetComponentLocation());
    TempBullet->SetShotDirection(this->DirectionShot);
    TempBullet->FinishSpawning(TempTransform);

    UE_LOG(LogSkeletonCannonPawn, Display, TEXT("Skeleton Cannon: %s | Bullet: %s | Location: %s | is spawning"), *GetName(),
        *TempBullet->GetName(), *TempBullet->GetActorLocation().ToString());
}

```
  
For more flexible the `ASkeletonCannonPawn` class management public functions are defined: `void StartFireCannon();` and `void StopFireCannon();` :
```

void ASkeletonCannonPawn::StopFireCannon()
{
    ChangeStateBrain(EStateBrain::Idle);
    GetWorldTimerManager().ClearTimer(this->TimerAnimFR);
}

void ASkeletonCannonPawn::StartFireCannon()
{
    ChangeStateBrain(EStateBrain::Reload);
    GetWorld()->GetTimerManager().SetTimer(this->TimerAnimFR, this, &ASkeletonCannonPawn::SwapAnimState, this->TimeRateReload, false);
}

```
  


[UP Table of contents](#main)  

## 📫 Other <a name="Other"></a>
:bangbang: Attention: If you can improve my trash code then make a pull request.  

**:copyright:Authors:**  
  
*[Mark Veligod](https://github.com/markveligod)*  
