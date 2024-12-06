// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

//class AGun;
class UCameraComponent;

UCLASS()
class UE5_SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// calculate the damage direction comes from
	FVector CalculateDamageDirection(AActor* DamageCauser);

	UPROPERTY(BlueprintReadWrite)
    bool bIsHit = false;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float HealthRatio() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector2D DamageDirectionForAnimation;

	// enable the get damage number output widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DamageWidgetClass;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// called to shoot
	void Shoot();

	// Switch Weapon
	void SwitchWeapon();

private:
	void MoveForward(float AxisValue);
	void LookUp(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);

	void SwitchWeaponMesh(int index);
	

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float HP = 0.0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> WeaponAClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> WeaponBClass;

	// Array for weapons
	UPROPERTY()
	TArray<AGun*> Weapons;

	int32 WeaponIndex = 0;

	UPROPERTY()
	class AGun* Gun;

	FTimerHandle TimeHandle;

	void Is_NotHit() ;

public:
	// enable the Shooting vibration
	void ShootingVibration();

	// enable the moving vibration
	void MovingVibration();

	// enable get shoot vibration
	void GetHitVibration();

private:
	// enable the camera shake 
	UPROPERTY(EditAnywhere, category = "combat")
	TSubclassOf< class UCameraShakeBase> HitCameraShakeClass;

	bool bIsMoving; // Tracks if the character is actively moving

private:
	void SetDamageValue(UUserWidget* DamageWidget, float GetDamage);

	void PlayRandomDynamicDamageWidgetAni(UUserWidget* DamageWidget);

};
