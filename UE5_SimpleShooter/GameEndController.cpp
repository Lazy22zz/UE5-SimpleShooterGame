// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndController.h"
#include "Blueprint/UserWidget.h"

void AGameEndController::BeginPlay()
{
    Super::BeginPlay();
    CrossHairScreenClass = CreateWidget(this, HUDClass);
        if (CrossHairScreenClass != nullptr)
        {
            CrossHairScreenClass -> AddToViewport();
        }
}

void AGameEndController::GameHasEnded(AActor *EndGameFocus, bool bIsWinner)
{
    // Winner is Player Or AI
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    CrossHairScreenClass -> RemoveFromParent();

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
    

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

