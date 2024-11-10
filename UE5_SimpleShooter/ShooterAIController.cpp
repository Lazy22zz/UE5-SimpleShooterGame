// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    /* 
    
    SetFocus(FocusPawn, 2);
    */

   if(AIEnemyBehaviour)
   {
    RunBehaviorTree(AIEnemyBehaviour);
    FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetBlackboardComponent() -> SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
    GetBlackboardComponent() -> SetValueAsVector(TEXT("StartLocation"), FocusPawn -> GetActorLocation());
   }
}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FocusPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (LineOfSightTo(FocusPawn))
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), FocusPawn -> GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), FocusPawn -> GetActorLocation());
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }

}