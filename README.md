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
