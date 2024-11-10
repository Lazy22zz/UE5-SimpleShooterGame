// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    /* 
    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    SetFocus(FocusPawn, 2);
    */

   if(AIEnemyBehaviour)
   {
    RunBehaviorTree(AIEnemyBehaviour);
    AActor* FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetBlackboardComponent() -> SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
   }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    /*
    

    if (LineOfSightTo(FocusPawn))
    {
        SetFocus(FocusPawn);
        MoveToActor(FocusPawn, AcceptanceRadius);
    }
    else
    {
        ClearFocus(2);
        StopMovement();
    }

    */
}