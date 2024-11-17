// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	float GetDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
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
	PlayerInputComponent -> BindAction(TEXT("ControllerVibration"), EInputEvent::IE_Released, this, &AShooterCharacter::ControllerVibration);
	
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

void AShooterCharacter::ControllerVibration()
{
	APlayerController* PController = Cast<APlayerController>(GetController());

	if (PController)
	{
		PController -> PlayDynamicForceFeedback(1.0f, 0.4f, true, true, true, true);
	}
}
