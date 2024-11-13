// Fill out your copyright notice in the Description page of Project Settings.


#include "KilleAllEmyGameModeBase.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"


void AKilleAllEmyGameModeBase::PawnKilled(APawn *Pawn)
{
    Super::PawnKilled(Pawn);
    //UE_LOG(LogTemp, Warning, TEXT("A Pawn is Killed!"));
    
    APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
    if (PlayerController != nullptr)
    {
        GameEnd(true);
    }

}

void AKilleAllEmyGameModeBase::GameEnd(bool BIsPlayerWinner)
{
   for (AController* Controller : TActorRange<AController>(GetWorld()))
   {
        bool bIsWinner = Controller->IsPlayerController() == BIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
   } 
}
