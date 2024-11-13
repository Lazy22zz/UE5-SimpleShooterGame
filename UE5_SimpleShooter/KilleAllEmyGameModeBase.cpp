// Fill out your copyright notice in the Description page of Project Settings.


#include "KilleAllEmyGameModeBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKilleAllEmyGameModeBase::PawnKilled(APawn *Pawn) // Pawn means the AI enemy
{
    Super::PawnKilled(Pawn);
    
    APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
    if (PlayerController != nullptr)
    {
        GameEnd(false);
    }

    for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
    {
        if (!Controller->Is_Dead())
        {
            return;
        }
    }

    GameEnd(true);

}

void AKilleAllEmyGameModeBase::GameEnd(bool BIsPlayerWinner)
{
   for (AController* Controller : TActorRange<AController>(GetWorld()))
   {
        bool bIsWinner = Controller->IsPlayerController() == BIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
   } 
}
