// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh -> SetupAttachment(root);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(GunSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);

	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactGunSound, Hit.Location);
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			float AppliedDamage = Gun_Damage;
			UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *Hit.Component->GetName());

			if (Hit.Component -> GetName() == TEXT("HeadCollision"))
			{
				AppliedDamage = AppliedDamage * 100000.f;
			}
			else if (Hit.Component -> GetName() == TEXT("BodyCollision"))
			{
				AppliedDamage = AppliedDamage * 0.5f;
			}
			else if (Hit.Component -> GetName() == TEXT("FootCollision"))
			{
				AppliedDamage = AppliedDamage * 0.2f;
			}
			else
			{
				AppliedDamage = AppliedDamage * 0.1f;
			}

			// grab the damage value;
			GetDamage = AppliedDamage;
			
			FPointDamageEvent DamageEvent(AppliedDamage, Hit, ShotDirection, nullptr);
			AController *OwnerController = GetOwnerController();
			HitActor->TakeDamage(AppliedDamage, DamageEvent, OwnerController, this);
		}
	}
}

bool AGun::GunTrace(FHitResult &Hit, FVector& ShotDirection) 
{
	AController *OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange_Bullet;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	TArray<FHitResult> Hits;
    bool bHitSomething = GetWorld()->LineTraceMultiByChannel(Hits, Location, End, ECC_GameTraceChannel1, Params);

    if (bHitSomething)
    {
        for (const FHitResult& SingleHit : Hits)
        {
            if (SingleHit.Component->GetName() == TEXT("HeadCollision"))
            {
                Hit = SingleHit;
                return true; // Break the loop when the desired component is hit
            }
			else if (SingleHit.Component->GetName() == TEXT("BodyCollision"))
            {
                Hit = SingleHit;
                return true; // Break the loop when the desired component is hit
            }
			else if (SingleHit.Component->GetName() == TEXT("FootCollision"))
            {
                Hit = SingleHit;
                return true; // Break the loop when the desired component is hit
            }
        }
        // If no special collision was hit, fall back to the first valid hit
        Hit = Hits[0];
    }

    return bHitSomething;
}

AController* AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}


// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

