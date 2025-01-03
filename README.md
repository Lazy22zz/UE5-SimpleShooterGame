# This is project(NOTE)
# HOTKEYS:
- F8, see the relate header
- F12, see the define function
- ctr + shift+ p, quick implement a function.
- ctr + p, find the file
- ctr+shift+o, find the specific function
# 1, Add Movement Function
```c++
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent -> BindAxis (TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent -> BindAxis (TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent -> BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent -> BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent -> BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

```
- Actually, we can use class relationship to avoid some duplicate functions
  ```c++
  PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
  PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
  ```
# 2，creating the controller aiming
- For Framerates and Axes
- Mouse : 30 FPS: 10, 10, 10
- Mouse : 60 FPS: 5, 5, 5, 5, 5, 5
- Controller : 30 FPS: 1, 1, 1
- Controller : 60 FPS: 1,1,1,1,1,1
- **SOLUTION**: 
```c++
// three directions: Roll, Pitch, Yaw, replace by "___"
AddController___Input(AxisValue * Rate * GetWorld()->GetDeltaSeconds() )
```
# 3, Adding Camera and Spring Arm
- Socket can enbale the rotate the springarm, Camera Settings -> Use Pawn Controller Rotate enables camera rotate by pawn movement
# 4, Add Animation
- Mesh, Animation is decided by the Skeletal.
# 5, Edit Collision Issue
- Find the meshes, go to show, click the simple collision, delete the wrong one.
# 6, Edit the Animation with BLEND
- create a new animation blueprint
- ![屏幕截图 2024-10-31 100319](https://github.com/user-attachments/assets/e4d90c46-d607-4afb-b662-1367473becc5)
# 7, 2D Blend Space
- create a new blend space
- ![屏幕截图 2024-10-31 104023](https://github.com/user-attachments/assets/1e6fae28-e2f1-4e71-adfc-a8f1d08ba822)

# 8, Hook with the BS blueprint with the speed
- enable the character's mesh is the NEW BS blueprint
- ![屏幕截图 2024-11-01 100524](https://github.com/user-attachments/assets/33d8be2f-5ef9-4df5-a959-892ea2b51e9f)
- ![屏幕截图 2024-11-01 100530](https://github.com/user-attachments/assets/66fbdad6-e66f-48e9-9031-2b93206b7c97)
# 9, Hook with the BS blueprint with the angle
- because the difference between gobal and local, so we need to transform it.
- ![屏幕截图 2024-11-01 102153](https://github.com/user-attachments/assets/18213d72-5cff-4c2b-b81b-b680f5b78f54)
# 10, Calaculate the velocity
 of foot slide
- using formula : foot_speed = (y_finish - y_start) / (t_finish - t_start)
- capcture the all info from the animation, y is the world position, (need to transform in Bone settings), t is time
- ![屏幕截图 2024-11-01 105526](https://github.com/user-attachments/assets/f846c0a1-85c6-48d5-a079-10f49cb7ee98)
# 11, Create a Gun Actor
- create a actor c++, and create a new blueprint
- because the mesh is skeletalmesh, so dont use basic meshcomponent.
  ```c++
	  	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
		SetRootComponent(root);
	
		Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
		Mesh -> SetupAttachment(root);
   ```
  # 12, Spwan the actor when the game start
  - in shootercharacter.cpp
    ```c++
    Gun = GetWorld() -> SpawnActor<AGun>(GunClass);
    ```
  - in shooterCharacter.cpp
    ```c++
    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;
# 13, replace the weapon 
- go to the skeleton, find the weapon's name, hide it
- ```c++
  ENGINE_API void HideBoneByName( FName BoneName, EPhysBodyOp PhysBodyOption );
    ```
- find the attached skeleton, add a new socket
- ```c++
  Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Socket"));
  ```
- setowner fro the weapon
- ```c++
  Gun->SetOwner(this);
  ```
- adjust the weapon position.
# 14, Shooting Architure
- create a pulltrigger() in gun file
 ```c++
  void AGun::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("You Pull The Trigger!"));
}
```
- enable character bind the action, gun object is created, so use it.
```c++
PlayerInputComponent -> BindAction (TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
```
```c++
void AShooterCharacter::Shoot()
{
	Gun -> PullTrigger();
}
```
# 15, add a muzle for the gun
```c++
static ENGINE_API UParticleSystemComponent* SpawnEmitterAttached(class UParticleSystem* EmitterTemplate, class USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector(ForceInit), FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f), EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bAutoDestroy = true, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None, bool bAutoActivate=true);
```
- replace the ue_log
```c++
UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
```
- add the muzzle
# 16, enable the player view point (prepare for the ray tracing for gun)
- use the drawdebughelper.h
```c++
ENGINE_API void DrawDebugCamera(const UWorld* InWorld, FVector const& Location, FRotator const& Rotation, float FOVDeg, float Scale=1.f, FColor const& Color=FColor::White, bool bPersistentLines=false, float LifeTime=-1.f, uint8 DepthPriority = 0);
```
- but this is the view camera for the gun, we need to change the view to the Acontroller (which can find in Controller.cpp)
- using cast to convert to the pawn
- use this pawn to getcontroller
```c++
void AController::GetPlayerViewPoint( FVector& out_Location, FRotator& out_Rotation ) const
```
# 17, Line Tracing By Channel
- Engine- collison
- Object Channel: Object channels are used to define the type of an object in the game world.
- Trace Channel: Trace channels are used for line, sphere, or shape traces (commonly known as raycasting) to check for collisions or overlaps. Traces help determine what kind of objects a trace "hits" or interacts with in the scene.
- use trace channel for bullet
- enable some specific collision preset
- in code, world.h
```c++
bool LineTraceSingleByChannel(struct FHitResult& OutHit,const FVector& Start,const FVector& End,ECollisionChannel TraceChannel,const FCollisionQueryParams& Params = FCollisionQueryParams::DefaultQueryParam, const FCollisionResponseParams& ResponseParam = FCollisionResponseParams::DefaultResponseParam) const;
```
- to find the ECollisionChannel, go to folder -> config -> DefaultEngine.ini
- search for bullet (just created)
```c++
DefaultChannelResponses=(Channel=ECC_GameTraceChannel1,DefaultResponse=ECR_Block,bTraceType=True,bStaticObject=False,Name="Bullet")
```
- use the drawdebugpoint
```c++
DrawDebugPoint(GetWorld(), Location, 2, FColor::Red, true);
```
- the final will belike
```c++
	FVector start = Location;
	FVector end = Location + Rotation.Vector() * MaxRange_Bullet;

	// TODO: LineTrace

	FHitResult OutHit;
	bool bsuccess = GetWorld() -> LineTraceSingleByChannel(OutHit, start, end, ECollisionChannel::ECC_EngineTraceChannel1);
	if (bsuccess)
	{
		DrawDebugPoint(GetWorld(), OutHit.Location, 20, FColor::Red, true);
	}
```
# 18, spwan the impact effects
```c++
static ENGINE_API UParticleSystemComponent* SpawnEmitterAtLocation(const UObject* WorldContextObject, UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation, bool bAutoDestroy, EPSCPoolMethod PoolingMethod = EPSCPoolMethod::None, bool bAutoActivateSystem =true);
```
- remember the rotation should be negative.
```c++
FVector ShotDirection = -Rotation.Vector();
UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, OutHit.Location, ShotDirection.Rotation());
```
# 19, Take Damage
- In Actor.h
```c++
ENGINE_API virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
```
- In DamageEvents.h
```c++
FPointDamageEvent(float InDamage, const FHitResult& InHitInfo, FVector const& InShotDirection, TSubclassOf<UDamageType> InDamageTypeClass)
		: FDamageEvent(InDamageTypeClass), Damage(InDamage), ShotDirection(InShotDirection), HitInfo(InHitInfo)
```
- Dont forget to include the header
```c++
		FPointDamageEvent DamageEvent(Gun_Damage, OutHit, ShotDirection, nullptr);
		AActor* HitActor = OutHit.GetActor();
		if (HitActor)
		{
			HitActor -> TakeDamage(Gun_Damage, DamageEvent, OwnerController, this);
		}
```
# 20, VIRTUAL METHODS IN C++
- Virtual : create polymorphic behavior in classes, allowing functions to be overridden in derived classes.
- Override : enable the programmer to see which functions are overriden. 
- Super:: : helping the override functions re-use.
- Implement the takedamage for actor in shootercharacter.cpp
```c++
float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GetDamage = FMath::Min(HP, GetDamage);
	HP = HP - GetDamage;
	UE_LOG(LogTemp, Warning, TEXT("Health Left: %f"), HP);

	return GetDamage;
}
```
# 21, Add blend poss by bool
- add a new branch, create a new variable: Is_Dead
- for the dead_Forward animation, stop the loop animation.
# 22, Add pure function to enable deadplay animation
```c++
UFUNCTION(BlueprintPure)
bool IsDead() const;
```
- in blueprint, cast pawn to shooter_character, then get Is_Dead and set Is_Dead
# 23, Create a AI controller
- create a bp subclass
- enable the new bp-aicontroller for the pawn(change the ai_controller of shooter character)
# 24, enable AI aiming
- In AIcontroller.h
```c++
AIMODULE_API virtual void SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);
AIMODULE_API virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay);
```
- we have two options:
- 1, using Gameplaystatic::GetPlayerPawn(), which will return AActor
- 2, using AActor::GetPlayerLocation(), which will return FVector
```c++
void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    SetFocus(FocusPawn, 2);
}
```
# 25, AI following
- create a NavMeshBound too enable to do A* Algorithm path searching
- using AiController.h
```c++
AIMODULE_API EPathFollowingRequestResult::Type MoveToActor(AActor* Goal, float AcceptanceRadius = -1, bool bStopOnOverlap = true,
		bool bUsePathfinding = true, bool bCanStrafe = true,
		TSubclassOf<UNavigationQueryFilter> FilterClass = NULL, bool bAllowPartialPath = true);
```
- Put it in Tick() function
# 26, Stop AI following
- using AIcontroller.h
```c++
AIMODULE_API virtual void ClearFocus(EAIFocusPriority::Type InPriority);
AIMODULE_API virtual void StopMovement() override;
```
- the new one will be like
```c++
void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (LineOfSightTo(FocusPawn))
    {
        SetFocus(FocusPawn);
        MoveToActor(FocusPawn, AcceptanceRadius);
    }
    else
    {
        ClearFocus(2);
        StopMovement();
    }

}
```
# 27， using BehaviourTrees and Blackboard
- create BehaviourTree and BlackBoard
- in Aicontroller.h
```c++
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIEnemyBehaviour;
```
- Hook up the Behaviourtree in Bp_aicontroller
# 28, Editing BlackBoard
- in Aicontroller.h
```c++
UBlackboardComponent* GetBlackboardComponent() { return Blackboard; }
```
- in BlackBoardComponent.h
```c++
AIMODULE_API void SetValueAs___
```
- in AIcontroller.cpp
```c++
if(AIEnemyBehaviour)
   {
    RunBehaviorTree(AIEnemyBehaviour);
    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetBlackboardComponent() -> SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
   }
```
# 29, Modify the BehaviourTree
- ![屏幕截图 2024-11-10 111341](https://github.com/user-attachments/assets/803fb3a9-4d11-410b-9b26-9f11ff28c591)
- in AIcontroller.cpp
```c++
FVector StartLocation = GetPawn() -> GetActorLocation();
GetBlackboardComponent() -> SetValueAsVector(TEXT("StartLocation"), StartLocation);
```
# 30, Expand : Modify the Behaviour Tree
- ![屏幕截图 2024-11-10 115345](https://github.com/user-attachments/assets/2e36f209-9938-4d56-9113-0fb4488c8499)
- Hints: right click the Sequence, pick the BlackBoard
- Hints: Click the BlackBoard Base Condition, choose the observour aborts : both
- In AIController.cpp
```c++
void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (LineOfSightTo(FocusPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), FocusPawn -> GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }

}
```
# 31, custom BTask in c++
- create a c++ for BTTask_blackboardbase
- in the UBTTask_ClearBlackboardBaseValue.cpp
```c++
UBTTask_ClearBlackboardBaseValue::UBTTask_ClearBlackboardBaseValue()
{
    NodeName = "Clear Blackboard Value";
}
```
- And add some Blackboard keys in the blackboard node
- ![屏幕截图 2024-11-10 140105](https://github.com/user-attachments/assets/807f9f6a-b531-497d-9d02-718f290e387f)
# 32, Excutting the Bttask (*IMPORTANT*)
- Purpose: clear all previous action 
- in bttasknode.h
```c++
AIMODULE_API virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
```
- Then, we nned to clear the blackboard componment
- in attask_blackboardbase.h
```c++
/** get name of selected blackboard key */
AIMODULE_API FName GetSelectedBlackboardKey() const;
```
- In AIController.h
```c++
const UBlackboardComponent* GetBlackboardComponent() const { return Blackboard; }
```
- In bttask_clearblackboardvalue.h
```c++
EBTNodeResult::Type UBTTask_ClearBlackboardBaseValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;

}
```
# EXTRA: DIFFERENCE BETWEEN BTTASKBLACKBOARDNODE AND BTTASKBLACKBOARDBASE
- Use UBTTaskNode if your task is more general-purpose and does not need to reference a specific Blackboard key.
- Use UBTTask_BlackboardBase if your task directly interacts with a Blackboard key, as it simplifies code and setup in the Behavior Tree Editor.
# 33, Create a Shoot() blackboardnode(*important*)
- enable the shoot() in shootercharacter.h to be public
- in braincomponent.h
```c++
FORCEINLINE AAIController* GetAIOwner() const { return AIOwner; }
```
- class UBehaviorTreeComponent : public UBrainComponent
- then, we can use cast <>() to get the AShooter class to enable to use getpawn()
- in AIcontroller.h
```c++
/** Getter for Pawn */
FORCEINLINE APawn* GetPawn() const { return Pawn; }
```
- adjusted:
```c++
EBTNodeResult::Type UBT_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter* AICharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AICharacter)
    {
        AICharacter -> Shoot();
    }
    return EBTNodeResult::Succeeded;
}
```
- `HINT`: in blackboard's (Node: Move to), check the observed blackboard value will auto update when the player character moves, the playerLocation will update in each frame
# 34， Add BTservice(*important*)
- Goal: simplify the BehaviourTree, which not need to use AI controller to do update Player location
- right click the sequence, add a default focus, which will auto focus the set player location
- Create a BTservice c++
- In BTService_PlayerLocation.cpp
```c++
UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = "Update Player Location";
}
```
- In BTservice.h, using the TickNode {auto update the location value}
```c++
Because some of them can be instanced for specific AI, following virtual functions are not marked as const:
 *   - OnBecomeRelevant (from UBTAuxiliaryNode)
 *   - OnCeaseRelevant (from UBTAuxiliaryNode)
 *   - TickNode (from UBTAuxiliaryNode)
 *   - OnSearchStart
```
- call the Super::TickNode()
- Similiar as the ShooterAIController.cpp
```c++
GetBlackboardComponent() -> SetValueAsVector(TEXT("StartLocation"), FocusPawn -> GetActorLocation());
```
- we need to use `GetSelectedBlackboardkey()` to replace the `TEXT()`
```c++
void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (FocusPawn == nullptr)
    {
        return;
    }

    OwnerComp.GetBlackboardComponent() -> SetValueAsVector(GetSelectedBlackboardKey(), FocusPawn -> GetActorLocation());
}
```
- And we need to replace the outofsight function
```c++
void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (FocusPawn == nullptr)
    {
        return;
    }

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    if (AIController -> LineOfSightTo(FocusPawn))
    {
        OwnerComp.GetBlackboardComponent() -> SetValueAsVector(GetSelectedBlackboardKey(), FocusPawn -> GetActorLocation());
    }
    else
    {
        OwnerComp.GetBlackboardComponent() -> ClearValue(GetSelectedBlackboardKey());
    }
}
```
- Then add those BTServices into the BehaviourTree
- *IMPORTANT*: we can use `OwnerComp.GetAIOwner()` to find the pawn()
# 35, Fixing the issue
- 1, Fix the enemy shooting itself
- Reason: the capsult. Becasue the bullet starts from the eye, its capsult, gun will get in touch with the character.
- In Gun.cpp
```c++
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool bsuccess = GetWorld() -> LineTraceSingleByChannel(OutHit, start, end, ECollisionChannel::ECC_GameTraceChannel1, Params);
```
- 2, If pawn is died, the collision is still existed, if it is enemy, its still shooting
- Reason: a, controller still on the pawn; b, capsult is enabled
- In ShooterCharacter.cpp, TakeDamage()
```c++
	if (IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
```
# 36, Ending the Game
- ![屏幕截图 2024-11-11 102750](https://github.com/user-attachments/assets/24b6ff91-ac4d-4d42-b1d9-d55df1bd61da)
- Create a new c++ KillEmAllGameMode inherited the Simpleshootergamemode
- In simpleShooterGameMode.h
- create a public function, `PawnKilled()`
- in KillEmAllGameMode.h, enable the virtual function `PawnKilled()`
- Then, if a pawn is skilled, we need to call the `PawnKilled()`
- In ShooterCharacter.cpp 's TakeDamage()
```c++
ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
```
- in project setting, changes its gamemode to KillenAllgamemode
- In `BP_KilleAllEmyGameModeBase`, confirm the parent class is `KilleAllEmyGameModeBase`.
# 37, Game Ended and create a Timer
- In KillAllEmyGameModeBase.cpp
- using the GameHasEned() from APlayerController.h
```c++
ENGINE_API virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
```
- Create a new playercontroller `GameEndController`
- override the `GameHasEnded()`, (hook with the BP_KilleAllEmyGameModeBase)
- the we get an error:
```c++
LoginId:3a831edf45c5b0eb2ea9feaa51bd2ff4
EpicAccountId:86a98e40ec2d4767bbe8137910abc3bc

Unhandled Exception: EXCEPTION_ACCESS_VIOLATION reading address 0x0000000000000000

UnrealEditor_UE5_SimpleShooter_4548!AKilleAllEmyGameModeBase::PawnKilled() [C:\Doucement\UE5_SimpleShooter\Source\UE5_SimpleShooter\KilleAllEmyGameModeBase.cpp:12]
UnrealEditor_UE5_SimpleShooter_4548!AShooterCharacter::TakeDamage() [C:\Doucement\UE5_SimpleShooter\Source\UE5_SimpleShooter\ShooterCharacter.cpp:51]
UnrealEditor_UE5_SimpleShooter_4548!AGun::PullTrigger() [C:\Doucement\UE5_SimpleShooter\Source\UE5_SimpleShooter\Gun.cpp:57]
```
- In order to fix it,
- In shootercharacter.cpp, `DetachFromControllerPendingDestroy();` enable the controller is nullptr.
- So rerrange the order
- Set the Timer:
- In TimerManager.h
```c++
FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f)
```
- `FTimerDelegate const& InDelegate` is call back the restartlevel() in TimerManager.h
- In GameEndController.h
```c++
GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
```
# 38, Create a LossGameWidget
- In UserWidget.h
```c++
WidgetT* CreateWidget(OwnerType OwningObject, TSubclassOf<UUserWidget> UserWidgetClass = WidgetT::StaticClass(), FName WidgetName = NAME_None)
```
- then , using
```c++
UUserWidget -> AddToViewport();
```
- After compiling, you will get an error.
```c++
[1/4] Compile [x64] GameEndController.cpp
C:\Doucement\UE5_SimpleShooter\Source\UE5_SimpleShooter\GameEndController.cpp(5): fatal error C1083: Cannot open include file: 'Blueprint/UserWidget.h': No such file or directory
[2/4] Compile [x64] GameEndController.gen.cpp
C:\Doucement\UE5_SimpleShooter\Intermediate\Build\Win64\UnrealGame\Inc\UE5_SimpleShooter\UHT\GameEndController.gen.cpp(17): error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
C:\Doucement\UE5_SimpleShooter\Intermediate\Build\Win64\UnrealGame\Inc\UE5_SimpleShooter\UHT\GameEndController.gen.cpp(17): error C2146: syntax error: missing ';' before identifier 'UClass'
C:\Doucement\UE5_SimpleShooter\Intermediate\Build\Win64\UnrealGame\Inc\UE5_SimpleShooter\UHT\GameEndController.gen.cpp(59): error C2065: 'Z_Construct_UClass_UUserWidget_NoRegister': undeclared identifier
C:\Doucement\UE5_SimpleShooter\Intermediate\Build\Win64\UnrealGame\Inc\UE5_SimpleShooter\UHT\GameEndController.gen.cpp(59): error C2737: 'NewProp_LoseScreenClass': const object must be initialized
```
- In order to fix it, go to the UE5_SimpleShooter.Build.cs
- Add `UMG` in
```c#
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
```
- In BP_GameEndController, change the loss game class to WBP_LossGameWidget
# 39, Redesign the GameEnd() function (*IMPORTANT*)
- purpose: designed to end the game for all controllers (both player and AI) and set a winning or losing state based on whether they match the `BIsPlayerWinner` parameter.
- In EngineUtils.h
- using
```c++
for (AController* Controller : TActorRange<AController>(GetWorld()))
   {
        bool bIsWinner = Controller->IsPlayerController() == BIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
   } 
```
- 1, check whether the winner is player or AI
- 2, lock the playerloss scene view in `Controller->GetPawn()`
- 3, show the who is the winner
# 40, Enable the win widget and finish all win / lose Ui setting (*IMPORTANT*)
- 1, create the winscreenwidget, if `bIsWinner` == player, then create the win widget; if not, create the lose widget
```c++
if (bIsWinner)
    {
        UUserWidget* LossGameScreenWidget = CreateWidget(this, WinScreenClass);
        if (LossGameScreenWidget != nullptr)
        {
            LossGameScreenWidget -> AddToViewport();
        }
    }
    else
    {
        UUserWidget* LossGameScreenWidget = CreateWidget(this, LoseScreenClass);
        if (LossGameScreenWidget != nullptr)
        {
            LossGameScreenWidget -> AddToViewport();
        }
    }
```
- 2, Identify how to decide game win/lose
- a, In UE5_TankGame, we do use to calculate all enemy number, and then if == 0, then win.
- the cons is : if we change the gamemode often, then we need to fix the `SYNC`
- b, In the AIcharacter.h, create a IsDead()function, seeking if this character is dead
- pros: No need to run whole level, just check each enemy is alive.
- In AIController.h
```c++
bool AShooterAIController::Is_Dead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return false;
}
```
- 3, In KilleAllEmGmaeBaseMode.cpp
- we need a global loop to see if AI all dies, then gameend.
```c++
void AKilleAllEmyGameModeBase::PawnKilled(APawn *Pawn) // Pawn means the AI enemy
{
    Super::PawnKilled(Pawn);
    
    APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
    if (PlayerController != nullptr)
    {
        GameEnd(false);
    }

    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->Is_Dead())
        {
            return;
        }
    }

    GameEnd(true);
}
```
# 42, re-organize the Gun Trace
- check the changes of code in Nov 13, 2024 
- In commit a78c95c
# 43, Add sound Effect
- 1, Enable the usage of soundbase in Details
- similiar the `#15, add a muzle for the gun`
- In Gameplaystatics.h
```c++
static ENGINE_API UAudioComponent* SpawnSoundAttached(USoundBase* Sound, USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector(ForceInit), FRotator Rotation = FRotator::ZeroRotator, EAttachLocation::Type LocationType = EAttachLocation::KeepRelativeOffset, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, USoundAttenuation* AttenuationSettings = nullptr, USoundConcurrency* ConcurrencySettings = nullptr, bool bAutoDestroy = true);
```
- And
```c++
static ENGINE_API void PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, FRotator Rotation, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = nullptr, USoundConcurrency* ConcurrencySettings = nullptr, const AActor* OwningActor = nullptr, const UInitialActiveSoundParams* InitialParams = nullptr);
```
- which is
```c++
UGameplayStatics::SpawnSoundAttached(GunSound, Mesh, TEXT("MuzzleFlashSocket"));
UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactGunSound, Hit.Location);
```
- 2, Random the MultiSoundbase
- create a blueprint soundbase, select the multi soundbases, in the A_SoundBase, right click
- pick the random sounds, and add a modulator, then connect all together.
- 3, Create a sound spatialization, which seems more like nature 3D sound in different distance
- In A_soundcue, find the Attenuation,
- In Attenuation Settings, create a new sound Attenuation.
# 44, Add a new crossHair widget
- In GameEndController.cpp
```c++
void AGameEndController::BeginPlay()
{
    Super::BeginPlay();
    CrossHairScreenClass = CreateWidget(this, HUDClass);
        if (CrossHairScreenClass != nullptr)
        {
            CrossHairScreenClass -> AddToViewport();
        }
}
```
- And , we need to remove the crosshair widget when game ends
- `CrossHairScreenClass -> RemoveFromParent();` (IN UE5, We need to use RemoveFromParent())
# 45, Create a health bar
- In the crosshair widget, add progress bar
- In ShooterCharacter.cpp,
```c++
float AShooterCharacter::HealthRatio() const
{
    return HP/MaxHealth;
}
```
- then create a new bind, which connect from get player pawn -> cast to shootercharacter.
# 46, Fixing aiming issue
- What happen?: when rotate the controller, the player animation doesn't follow
- IN ABP_ShooterCharacter, add a new animation
- ![屏幕截图 2024-11-13 194752](https://github.com/user-attachments/assets/c627af42-1936-4d19-bf11-24829f5e5ca2)
- In the Event Graph, using the subtract(get controler rotation - get player rotation)
- Fix: The AI aiming from two different height position issue
- In BT_Tree, change the player location to player, object
- In BTService_PlayerLocationSeen.cpp
```c++
OwnerComp.GetBlackboardComponent() -> SetValueAsObject(GetSelectedBlackboardKey(), FocusPawn);
```
# 47, State Machine
- Purpose : Enable switch two animations in a condition: change grounded animation to inair animation.
- create a new static machine, add the required animation, hook the states.
- in the 
