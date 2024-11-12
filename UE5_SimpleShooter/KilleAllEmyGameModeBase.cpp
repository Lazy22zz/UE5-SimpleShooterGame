// Fill out your copyright notice in the Description page of Project Settings.


#include "KilleAllEmyGameModeBase.h"
#include "TimerManager.h"

void AKilleAllEmyGameModeBase::PawnKilled(APawn *Pawn)
{
    Super::PawnKilled(Pawn);
    //UE_LOG(LogTemp, Warning, TEXT("A Pawn is Killed!"));
    
    APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
    if (PlayerController != nullptr)
    {
        PlayerController->GameHasEnded(nullptr, false);
    }

}
