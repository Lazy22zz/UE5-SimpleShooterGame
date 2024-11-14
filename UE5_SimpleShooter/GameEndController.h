// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameEndController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_SIMPLESHOOTER_API AGameEndController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor *EndGameFocus, bool bIsWinner) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;

	UUserWidget* CrossHairScreenClass;

};
