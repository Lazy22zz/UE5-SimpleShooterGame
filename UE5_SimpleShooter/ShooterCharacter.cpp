// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"




// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bIsMoving = false; // Not moving at the start

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHealth;

	WeaponIndex = 0;

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	DefaultWeaponClass = WeaponAClass;
	Gun = GetWorld() -> SpawnActor<AGun>(DefaultWeaponClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Socket"));
	Gun->SetOwner(this);
	
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{

	bIsHit = true;

	//player camera shake
	GetWorld() -> GetFirstPlayerController() -> ClientStartCameraShake(HitCameraShakeClass);

	//get get hit vibration
	GetHitVibration();

	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//UE_LOG(LogTemp, Warning, TEXT("DisplayDamageWidget called!"));

	if (GetDamage > 0.0f && DamageWidgetClass)
	{
		UUserWidget* DamageWidget = CreateWidget<UUserWidget>(GetWorld(), DamageWidgetClass);
        if (DamageWidget)
        {
            DamageWidget->AddToViewport();

			// display the dynamicdamage ui
			DisplayDamageAni(DamageWidget, GetDamage);

			// Call the function to play a random animation
		    PlayRandomDynamicDamageWidgetAni(DamageWidget);

			// headshot vibration
			DetectHeadShotVibration(GetDamage);
			
        }
            
    }

	GetDamage = FMath::Min(HP, GetDamage);
	HP = HP - GetDamage;
	UE_LOG(LogTemp, Warning, TEXT("Health Left: %f"), HP);


	FVector DamageDirection = CalculateDamageDirection(DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Damage Direction: %s"), *DamageDirection.ToString());

	GetWorldTimerManager().SetTimer(
		TimeHandle,
		this, 
		&AShooterCharacter::Is_NotHit,
		1.0f,
		true
	);

	// Convert 3D direction to 2D for Blend Space
	DamageDirectionForAnimation = FVector2D(
		FVector::DotProduct(GetActorRightVector(), DamageDirection),  // X (Left/Right)
		FVector::DotProduct(GetActorForwardVector(), DamageDirection) // Y (Front/Back)
	);

	if (IsDead())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return GetDamage;
}

bool AShooterCharacter::IsDead() const
{
	bool Is_Dead = true;
	if (HP <= 0)
	{
		Is_Dead = true;
	}
	else{
		Is_Dead = false;
	}
	return Is_Dead;
}


// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent -> BindAxis (TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent -> BindAxis (TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent -> BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent -> BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent -> BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent -> BindAxis (TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate );
	PlayerInputComponent -> BindAxis (TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent -> BindAction (TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent -> BindAction(TEXT("SwitchWeapon"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SwitchWeapon);
	PlayerInputComponent -> BindAction(TEXT("ControllerVibration"), EInputEvent::IE_Released, this, &AShooterCharacter::ShootingVibration);
	
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER) // Detect significant input
    {
        bIsMoving = true;
        MovingVibration();
    }
    else
    {
        bIsMoving = false; // Stop movement when input ceases
    }

    AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER) // Detect significant input
    {
        bIsMoving = true;
        MovingVibration();
    }
    else
    {
        bIsMoving = false; // Stop movement when input ceases
    }

    AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld() -> GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun -> PullTrigger();
}

void AShooterCharacter::SwitchWeapon()
{
	WeaponIndex += 1;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Switching Weapons ..."));

    // Hide the currently equipped weapon
    if (Gun) 
    {
        Gun->SetActorHiddenInGame(true);
    }

	if (WeaponIndex % 2 == 0)
    {
        DefaultWeaponClass = WeaponAClass;
    }
    else
    {
        DefaultWeaponClass = WeaponBClass;
    }

    // Spawn the new weapon
    if (DefaultWeaponClass)
    {
        Gun = GetWorld()->SpawnActor<AGun>(DefaultWeaponClass);
        if (Gun)
        {
            Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Socket"));
            Gun->SetOwner(this);
            Gun->SetActorHiddenInGame(false); // Ensure the new weapon is visible
        }
    }
}

float AShooterCharacter::HealthRatio() const
{
    return HP/MaxHealth;
}

FVector AShooterCharacter::CalculateDamageDirection(AActor *DamageCauser)
{
	if (!DamageCauser)
    {
        return FVector::ZeroVector; // No direction if there's no damage causer
    }

	FVector DamageDirection = DamageCauser->GetActorLocation() - GetActorLocation();
	DamageDirection.Normalize();

    return DamageDirection;
}

void AShooterCharacter::Is_NotHit()
{
    bIsHit = false;
}

void AShooterCharacter::ShootingVibration()
{
	APlayerController* PController = Cast<APlayerController>(GetController());

	if (PController)
	{
		PController -> PlayDynamicForceFeedback(0.5f, 0.25f, true, true, true, true);
	}
}

void AShooterCharacter::MovingVibration()
{
    if (!bIsMoving) return; // Exit if no movement input

    static bool bLeftFoot = true; // Tracks which foot to vibrate
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return;

    if (bLeftFoot)
    {
        PlayerController->PlayDynamicForceFeedback(
            0.05f, 0.02f, true, true, false, false,
            EDynamicForceFeedbackAction::Start,
            FDynamicForceFeedbackHandle()
        );
    }
    else
    {
        PlayerController->PlayDynamicForceFeedback(
            0.05f, 0.02f, false, false, true, true,
            EDynamicForceFeedbackAction::Start,
            FDynamicForceFeedbackHandle()
        );
    }

    bLeftFoot = !bLeftFoot;
}

void AShooterCharacter::GetHitVibration()
{
	APlayerController* PController = Cast<APlayerController>(GetController());

	if (PController)
	{
		PController -> PlayDynamicForceFeedback(0.4f, 0.1f, true, true, true, true);
	}
}

void AShooterCharacter::GetHeadShotVibration()
{
	APlayerController* PController = Cast<APlayerController>(GetController());

	if (PController)
	{
		PController -> PlayDynamicForceFeedback(0.7f, 0.05f, false, true, false, true);
	}
}

 void AShooterCharacter::DisplayDamageAni(UUserWidget* DamageWidget, float GetDamage)
{
	UFunction* SetDamageTextFunction = DamageWidget->FindFunction(TEXT("DisplayWidget"));
	DamageWidget->ProcessEvent(SetDamageTextFunction, nullptr);
}


void AShooterCharacter::PlayRandomDynamicDamageWidgetAni(UUserWidget* DamageWidget)
{
	UFunction* PlayRandomAnimationFunction = DamageWidget->FindFunction(TEXT("PlayRandomAnimation"));
            if (PlayRandomAnimationFunction)
            {
                DamageWidget->ProcessEvent(PlayRandomAnimationFunction, nullptr);
            }
}

void AShooterCharacter::DetectHeadShotVibration(float damage)
{
	if (damage >= MaxHealth)
	{
		GetHeadShotVibration();
	}
}
