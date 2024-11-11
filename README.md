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
# 36,
