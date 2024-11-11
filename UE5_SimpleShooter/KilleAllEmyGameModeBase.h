// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "KilleAllEmyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5_SIMPLESHOOTER_API AKilleAllEmyGameModeBase : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* Pawn) override;
};
