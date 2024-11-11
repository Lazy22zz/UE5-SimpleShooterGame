// Fill out your copyright notice in the Description page of Project Settings.


#include "KilleAllEmyGameModeBase.h"

void AKilleAllEmyGameModeBase::PawnKilled(APawn *Pawn)
{
    Super::PawnKilled(Pawn);
    UE_LOG(LogTemp, Warning, TEXT("A Pawn is Killed!"));
}
