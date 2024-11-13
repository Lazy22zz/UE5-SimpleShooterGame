// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndController.h"
#include "Blueprint/UserWidget.h"

void AGameEndController::GameHasEnded(AActor *EndGameFocus, bool bIsWinner)
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);
    UE_LOG(LogTemp, Warning, TEXT("Game is Ended!"));

    UUserWidget* LossGameScreenWidget = CreateWidget(this, LoseScreenClass);
    if (LossGameScreenWidget != nullptr)
    {
        LossGameScreenWidget -> AddToViewport();
    }

    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
