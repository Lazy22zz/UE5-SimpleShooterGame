// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int, NewAmmoCount);

UCLASS()
class UE5_SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* GunSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactGunSound;

	UPROPERTY(EditAnywhere, Category = "GunProperty")
	float MaxRange_Bullet = 1000;

	UPROPERTY(EditAnywhere, Category = "GunProperty")
	int Num_Ammo = 100;

	UPROPERTY(EditAnywhere, Category = "GunProperty")
	float Gun_Damage = 10;

	bool GunTrace(FHitResult &Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;

public:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Damage")
	float GetDamage = 0.0f;

    // Expose the delegate to Blueprints
    UPROPERTY(BlueprintAssignable, Category = "Gun")
    FOnAmmoChanged OnAmmoChanged;
	// decrease bullet
	void DecreaseAmmo();
	
	// get bullet
	UFUNCTION(BlueprintCallable, Category = "GunProperty")
	int GetAmmoCount() const { return Num_Ammo; }


};
